#include "KarPlusStrong.cuh"
#include <device_launch_parameters.h>

#if CUDA_ENABLED

//滤波单位算子
__global__ void computeUnit(
	float* unitMemBase, const float* unitBase, int unitLength, int totalThread);

//衰减单位算子
__global__ void attenuateUnit(
	float* unitMemBase, int unitLength, float sCoefficient, float eCoefficient, int totalThread);

__host__ cudaError_t doSynthesisGPU(
	const float* part, int partSize,
	float* buffer, int bufferSize,
	const int* unitArray, int unitSize)
{
	//无需计算
	if (partSize <= 1) { return cudaError_t::cudaErrorUnknown; }
	if (bufferSize <= 1) { return cudaError_t::cudaErrorUnknown; }
	if (unitSize == 0) { return cudaError_t::cudaErrorUnknown; }

	//计算合成单元最多用到的长度
	int maxUnitLength = 0;
	for (int i = 0; i < unitSize; i++) {
		if (unitArray[i] > maxUnitLength) { maxUnitLength = unitArray[i]; }
	}
	if (maxUnitLength > partSize) { return cudaError_t::cudaErrorUnknown; }

	//选择设备
	int device = -1;
	cudaDeviceProp deviceProp = cudaDevicePropDontCare;
	if (auto result = cudaChooseDevice(&device, &deviceProp)) {
		return result;
	}
	if (auto result = cudaGetDeviceProperties(&deviceProp, device)) {
		return result;
	}
	if (auto result = cudaSetDevice(device)) {
		return result;
	}

	//在主机上申请合成结果储存空间
	float* hostBufferPtr = nullptr;
	if (auto result = cudaMallocHost(&hostBufferPtr, bufferSize * sizeof(float), cudaMemAttachGlobal)) {
		cudaDeviceReset();
		return result;
	}

	//在设备上申请原始合成单元储存空间
	float* devicePartPtr = nullptr;
	if (auto result = cudaMalloc(&devicePartPtr, maxUnitLength * sizeof(float))) {
		cudaFreeHost(hostBufferPtr);
		cudaDeviceReset();
		return result;
	}

	//在设备上申请合成单元滤波后序列的储存空间
	float* deviceUnitPtr = nullptr;
	if (auto result = cudaMalloc(&deviceUnitPtr, (size_t)unitSize * maxUnitLength * sizeof(float))) {
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaDeviceReset();
		return result;
	}

	//将原始合成单元存入设备
	if (auto result = cudaMemcpy(devicePartPtr, part, maxUnitLength * sizeof(float), cudaMemcpyHostToDevice)) {
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		cudaDeviceReset();
		return result;
	}

	//建立流
	cudaStream_t* streams = new cudaStream_t[unitSize];
	if (!streams) {
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		cudaDeviceReset();
		return cudaError_t::cudaErrorMemoryAllocation;
	}
	for (int i = 0; i < unitSize; i++) {
		cudaStreamCreateWithFlags(&streams[i], cudaStreamNonBlocking);
	}

	//建立同步事件
	cudaEvent_t* events = new cudaEvent_t[unitSize];
	if (!events) {
		//销毁流
		for (int i = 0; i < unitSize; i++) {
			cudaStreamDestroy(streams[i]);
		}
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		delete[] streams;
		cudaDeviceReset();
		return cudaError_t::cudaErrorMemoryAllocation;
	}
	for (int i = 0; i < unitSize; i++) {
		cudaEventCreateWithFlags(&events[i], cudaEventDisableTiming);
	}

	//等待同步
	cudaDeviceSynchronize();

	//获取设备属性
	int threadNumInABlock = deviceProp.maxThreadsPerBlock;

	//计算循环轮数
	int roundSize = (maxUnitLength / threadNumInABlock) + 1;

	//深度优先建立流计算任务
	int outDeviation = 0;
	for (int i = 0; i < unitSize; i++) {
		//在开始任务一前确认上一流已到达事件同步点
		if (i > 0) {
			cudaStreamWaitEvent(streams[i], events[i - 1]);
		}

		//任务一：对拼接单元依次低通滤波
		if (i == 0) {
			computeUnit <<<roundSize, threadNumInABlock, 0, streams[i]>>> (
				&deviceUnitPtr[i * maxUnitLength],
				&devicePartPtr[0], maxUnitLength, threadNumInABlock);
		}
		else {
			computeUnit <<<roundSize, threadNumInABlock, 0, streams[i]>>> (
				&deviceUnitPtr[i * maxUnitLength],
				&deviceUnitPtr[(i - 1) * maxUnitLength], maxUnitLength, threadNumInABlock);
		}

		//设置事件同步点
		cudaEventRecord(events[i], streams[i]);

		//在开始任务二前确认下一流已到达事件同步点
		if (i < unitSize - 1) {
			cudaStreamWaitEvent(streams[i], events[i + 1]);
		}

		//计算拼接单元长度
		int currentUnitLength = unitArray[i];

		//任务二：线性衰减
		int partRoundSize = (currentUnitLength / threadNumInABlock) + 1;
		attenuateUnit <<<partRoundSize, threadNumInABlock, 0, streams[i]>>> (
			&deviceUnitPtr[i * maxUnitLength], currentUnitLength,
			1.f - outDeviation / (float)(bufferSize - 1),
			1.f - (outDeviation + currentUnitLength - 1) / (float)(bufferSize - 1), threadNumInABlock);

		//任务三：将拼接单元拷贝到指定位置
		cudaMemcpyAsync(
			&hostBufferPtr[outDeviation], &deviceUnitPtr[i * maxUnitLength], currentUnitLength, cudaMemcpyDeviceToHost, streams[i]);
		outDeviation += currentUnitLength;
	}

	//等待同步
	cudaDeviceSynchronize();

	//销毁事件
	for (int i = 0; i < unitSize; i++) {
		cudaEventDestroy(events[i]);
	}
	delete[] events;

	//销毁流
	for (int i = 0; i < unitSize; i++) {
		cudaStreamDestroy(streams[i]);
	}
	delete[] streams;

	//释放多余的内存
	cudaFree(devicePartPtr);
	cudaFree(deviceUnitPtr);

	//将合成结果复制
	if (auto result = cudaMemcpy(buffer, hostBufferPtr, bufferSize * sizeof(float), cudaMemcpyHostToHost)) {
		cudaFreeHost(hostBufferPtr);
		cudaDeviceReset();
		return result;
	}

	//释放内存
	cudaFreeHost(hostBufferPtr);

	//重置设备
	cudaDeviceReset();
	
	//返回成功
	return cudaError_t::cudaSuccess;
}

__global__ void computeUnit(
	float* unitMemBase, const float* unitBase, int unitLength, int totalThread)
{
	//运算系数
	constexpr float r = 0.5f;

	//定位GPU线程
	int index = totalThread * blockIdx.x + threadIdx.x;

	//定位处理目标
	if (index >= unitLength || index < 0) { return; }
	int nextIndex = index - 1;
	if (index == 0) { nextIndex = unitLength - 1; }

	//平均
	unitMemBase[index] = unitBase[index] * r + unitBase[nextIndex] * (1.f - r);
}

__global__ void attenuateUnit(
	float* unitMemBase, int unitLength, float sCoefficient, float eCoefficient, int totalThread)
{
	//定位GPU线程
	int index = totalThread * blockIdx.x + threadIdx.x;

	//定位处理目标
	if (index >= unitLength || index < 0) { return; }

	//计算衰减系数
	float coefficient = sCoefficient + (eCoefficient - sCoefficient) * (index / (float)(unitLength - 1));

	//衰减
	unitMemBase[index] *= coefficient;
}

#endif