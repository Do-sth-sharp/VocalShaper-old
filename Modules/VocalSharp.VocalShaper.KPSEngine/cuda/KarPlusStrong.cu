#include "KarPlusStrong.cuh"
#include <device_launch_parameters.h>

#if CUDA_ENABLED

__host__ cudaError_t doSynthesis(
	const float* part, int partSize,
	float* buffer, int bufferSize,
	const int* unitArray, int unitSize)
{
	//使用设备0
	int device = 0;
	if (auto result = cudaSetDevice(device)) {
		return result;
	}

	//获取设备属性
	cudaDeviceProp deviceProp;
	if (auto result = cudaGetDeviceProperties(&deviceProp, device)) {
		return result;
	}

	//在设备上申请合成结果储存空间
	float* deviceBufferPtr = nullptr;
	if (auto result = cudaMalloc(&deviceBufferPtr, bufferSize * sizeof(float))) {
		return result;
	}

	//在设备上申请原始合成单元储存空间
	float* devicePartPtr = nullptr;
	if (auto result = cudaMalloc(&devicePartPtr, partSize * sizeof(float))) {
		cudaFree(deviceBufferPtr);
		return result;
	}

	//在设备上申请合成单元滤波后序列的储存空间
	float* deviceUnitPtr = nullptr;
	if (auto result = cudaMalloc(&deviceUnitPtr, (size_t)unitSize * partSize * sizeof(float))) {
		cudaFree(deviceBufferPtr);
		cudaFree(devicePartPtr);
		return result;
	}

	//将原始合成单元存入设备
	if (auto result = cudaMemcpy(devicePartPtr, part, partSize * sizeof(float), cudaMemcpyHostToDevice)) {
		cudaFree(deviceBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		return result;
	}

	//对合成单元滤波生成目标单元
	if (auto result = computeEachUnit(deviceProp, deviceUnitPtr, devicePartPtr, unitSize, partSize)) {
		cudaFree(deviceBufferPtr);
		cudaFree(devicePartPtr);
		cudaFree(deviceUnitPtr);
		return result;
	}

	//释放原始合成单元
	cudaFree(devicePartPtr);

	//TODO 单元拼接
	
	//返回成功
	return cudaError_t::cudaSuccess;
}

__host__ cudaError_t computeEachUnit(
	const cudaDeviceProp& prop,
	float* unitMem, const float* unit,
	int count, int unitLength)
{
	//无需计算
	if (unitLength == 0) { return cudaError_t::cudaErrorUnknown; }

	//获取设备属性
	auto& blockSizeInGrid = prop.maxGridSize;
	auto& threadSizeInBlock = prop.maxThreadsDim;
	int blockNumInAGrid = blockSizeInGrid[0] * blockSizeInGrid[1] * blockSizeInGrid[2];
	int threadNumInABlock = threadSizeInBlock[0] * threadSizeInBlock[1] * threadSizeInBlock[2];

	//计算block, thread用量与循环轮数
	int xSize = blockSizeInGrid[0], ySize = blockSizeInGrid[1], zSize = blockSizeInGrid[2];
	int txSize = threadSizeInBlock[0], tySize = threadSizeInBlock[1], tzSize = threadSizeInBlock[2];
	int roundSize = (unitLength / threadNumInABlock) / blockNumInAGrid + 1;
	if (roundSize == 1) {
		zSize = (unitLength / threadNumInABlock) / (blockSizeInGrid[0] * blockSizeInGrid[1]) + 1;
		if (zSize == 1) {
			ySize = (unitLength / threadNumInABlock) / blockSizeInGrid[0] + 1;
			if (ySize == 1) {
				xSize = (unitLength / threadNumInABlock) + 1;
				if (xSize == 1) {
					tzSize = unitLength / (threadSizeInBlock[0] * threadSizeInBlock[1]) + 1;
					if (tzSize == 1) {
						tySize = unitLength / threadSizeInBlock[0] + 1;
						if (tySize == 1) {
							txSize = unitLength;
						}
					}
				}
			}
		}
	}

	//规划算子
	dim3 grid(xSize, ySize, zSize), block(txSize, tySize, tzSize);

	//循环计算每一个单元
	for (int i = 0; i < count; i++) {
		if (i == 0) {
			for (int r = 0; r < roundSize; r++) {
				computeUnit <<<grid, block>>> (
					&unitMem[i * unitLength + r * blockNumInAGrid * threadNumInABlock],
					&unit[r * blockNumInAGrid * threadNumInABlock],
					unitLength);
			}
		}
		else {
			for (int r = 0; r < roundSize; r++) {
				computeUnit <<<grid, block>>> (
					&unitMem[i * unitLength + r * blockNumInAGrid * threadNumInABlock],
					&unitMem[(i - 1) * unitLength + r * blockNumInAGrid * threadNumInABlock],
					unitLength);
			}
		}
		cudaDeviceSynchronize();
	}

	return cudaError_t::cudaSuccess;
}

__global__ void computeUnit(
	float* unitMemBase, const float* unitBase, int unitLength)
{
	//定位GPU线程
	int blockIndex = blockIdx.z * (gridDim.x * gridDim.y) + blockIdx.y * gridDim.x + blockIdx.x;
	int blockSize = blockDim.x * blockDim.y * blockDim.z;
	int threadIndex = threadIdx.z * (blockDim.x * blockDim.y) + threadIdx.y * blockDim.x + threadIdx.x;
	int index = blockIndex * blockSize + threadIndex;

	//定位处理目标
	if (index >= unitLength || index < 0) { return; }
	int nextIndex = index + 1;
	if (index == unitLength - 1) { nextIndex = 0; }

	//平均
	unitMemBase[index] = unitBase[index] + (unitBase[nextIndex] - unitBase[index]) / 2;
}

#endif