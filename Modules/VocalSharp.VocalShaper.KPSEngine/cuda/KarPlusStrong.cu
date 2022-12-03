#include "KarPlusStrong.cuh"
#include <device_launch_parameters.h>

#if CUDA_ENABLED

__host__ cudaError_t doSynthesis(
	const float* part, int partSize,
	float* buffer, int bufferSize,
	const int* unitArray, int unitSize)
{
	//无需计算
	if (partSize <= 1) { return cudaError_t::cudaErrorUnknown; }
	if (bufferSize <= 1) { return cudaError_t::cudaErrorUnknown; }
	if (unitSize == 0) { return cudaError_t::cudaErrorUnknown; }

	//选择设备
	int device = -1;
	cudaDeviceProp deviceProp = cudaDevicePropDontCare;
	if (auto result = cudaChooseDevice(&device, &deviceProp)) {
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
	if (auto result = cudaMalloc(&devicePartPtr, partSize * sizeof(float))) {
		cudaFreeHost(hostBufferPtr);
		cudaDeviceReset();
		return result;
	}

	//在设备上申请合成单元滤波后序列的储存空间
	float* deviceUnitPtr = nullptr;
	if (auto result = cudaMalloc(&deviceUnitPtr, (size_t)unitSize * partSize * sizeof(float))) {
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaDeviceReset();
		return result;
	}

	//将原始合成单元存入设备
	if (auto result = cudaMemcpy(devicePartPtr, part, partSize * sizeof(float), cudaMemcpyHostToDevice)) {
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		cudaDeviceReset();
		return result;
	}

	//建立流
	cudaStream_t* streams = nullptr;
	if (auto result = cudaMalloc(&streams, unitSize * sizeof(cudaStream_t))) {
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		cudaDeviceReset();
		return result;
	}
	for (int i = 0; i < unitSize; i++) {
		cudaStreamCreateWithFlags(&streams[i], cudaStreamNonBlocking);
	}

	//建立同步事件
	cudaEvent_t* events = nullptr;
	if (auto result = cudaMalloc(&events, unitSize * sizeof(cudaEvent_t))) {
		//销毁流
		for (int i = 0; i < unitSize; i++) {
			cudaStreamDestroy(streams[i]);
		}
		cudaFreeHost(hostBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		cudaFree(streams);
		cudaDeviceReset();
		return result;
	}
	for (int i = 0; i < unitSize; i++) {
		cudaEventCreateWithFlags(&events[i], cudaEventDisableTiming);
	}

	//获取设备属性
	auto& threadSizeInBlock = deviceProp.maxThreadsDim;
	int threadNumInABlock = threadSizeInBlock[0] * threadSizeInBlock[1] * threadSizeInBlock[2];

	//计算循环轮数
	int roundSize = (partSize / threadNumInABlock) + 1;

	//规划算子
	dim3 block(threadSizeInBlock[0], threadSizeInBlock[1], threadSizeInBlock[2]);

	//深度优先建立流计算任务
	int outDeviation = 0;
	for (int i = 0; i < unitSize; i++) {
		//在开始任务一前确认上一流已到达事件同步点
		if (i > 0) {
			cudaStreamWaitEvent(streams[i], events[i - 1], cudaEventWaitExternal);
		}

		//任务一：对拼接单元依次低通滤波
		if (i == 0) {
			computeUnit <<<roundSize, block, 0, streams[i]>>> (
				&deviceUnitPtr[i * partSize],
				&devicePartPtr[0], partSize);
		}
		else {
			computeUnit <<<roundSize, block, 0, streams[i]>>> (
				&deviceUnitPtr[i * partSize],
				&deviceUnitPtr[(i - 1) * partSize], partSize);
		}

		//设置事件同步点
		cudaEventRecordWithFlags(events[i], streams[i], cudaEventRecordExternal);

		//在开始任务二前确认下一流已到达事件同步点
		if (i < unitSize - 1) {
			cudaStreamWaitEvent(streams[i], events[i + 1], cudaEventWaitExternal);
		}

		//计算拼接单元长度
		int currentUnitLength = unitArray[i];
		if (currentUnitLength > partSize) { currentUnitLength = partSize; }

		//任务二：线性衰减
		int partRoundSize = (currentUnitLength / threadNumInABlock) + 1;
		attenuateUnit <<<partRoundSize, block, 0, streams[i]>>> (
			&deviceUnitPtr[i * partSize], currentUnitLength,
			1.f - outDeviation / (float)(bufferSize - 1),
			1.f - (outDeviation + currentUnitLength - 1) / (float)(bufferSize - 1));

		//任务三：将拼接单元拷贝到指定位置
		cudaMemcpyAsync(
			&hostBufferPtr[outDeviation], &deviceUnitPtr[i * partSize], currentUnitLength, cudaMemcpyDeviceToHost, streams[i]);
		outDeviation += unitArray[i];
	}

	//等待同步
	cudaDeviceSynchronize();

	//销毁事件
	for (int i = 0; i < unitSize; i++) {
		cudaEventDestroy(events[i]);
	}
	cudaFree(events);

	//销毁流
	for (int i = 0; i < unitSize; i++) {
		cudaStreamDestroy(streams[i]);
	}
	cudaFree(streams);

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
	float* unitMemBase, const float* unitBase, int unitLength)
{
	//定位GPU线程
	int totalThread = blockDim.x * blockDim.y * blockDim.z;
	int threadIndex = threadIdx.z * (blockDim.x * blockDim.y) + threadIdx.y * blockDim.x + threadIdx.x;
	int index = totalThread * blockIdx.x + threadIndex;

	//定位处理目标
	if (index >= unitLength || index < 0) { return; }
	int nextIndex = index + 1;
	if (index == unitLength - 1) { nextIndex = 0; }

	//平均
	unitMemBase[index] = unitBase[index] + (unitBase[nextIndex] - unitBase[index]) / 2;
}

__global__ void attenuateUnit(
	float* unitMemBase, int unitLength, float sCoefficient, float eCoefficient)
{
	//定位GPU线程
	int totalThread = blockDim.x * blockDim.y * blockDim.z;
	int threadIndex = threadIdx.z * (blockDim.x * blockDim.y) + threadIdx.y * blockDim.x + threadIdx.x;
	int index = totalThread * blockIdx.x + threadIndex;

	//定位处理目标
	if (index >= unitLength || index < 0) { return; }

	//计算衰减系数
	float coefficient = sCoefficient + (eCoefficient - sCoefficient) * (index / (float)(unitLength - 1));

	//衰减
	unitMemBase[index] *= coefficient;
}

#endif