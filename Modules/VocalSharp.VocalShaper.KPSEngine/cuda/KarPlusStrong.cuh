#pragma once

#if CUDA_ENABLED

#include <cuda_runtime.h>

//调用此函数进行合成
//part: 用于合成的原始单元
//buffer: 目标存储位置
//unitArray: 合成单元序列中每一单元的目标长度
__host__ cudaError_t doSynthesis(
	const float* part, int partSize,
	float* buffer, int bufferSize,
	const int* unitArray, int unitSize);

//生成滤波后的合成单元
__host__ cudaError_t computeEachUnit(
	const cudaDeviceProp& prop,
	float* unitMem, const float* unit,
	int count, int unitLength);

//滤波单位算子
__global__ void computeUnit(
	float* unitMemBase, const float* unitBase, int unitLength);

#endif
