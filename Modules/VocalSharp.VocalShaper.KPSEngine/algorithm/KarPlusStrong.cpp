#include "KarPlusStrong.h"

//片段滤波
void computeUnit(
	float* unitMemBase, const float* unitBase, int unitLength);

//复制并衰减
void addUnit(
	float* unitMemBase, const float* unitBase, int unitLength, float sCoefficient, float eCoefficient);

int doSynthesisCPU(
	const float* part, int partSize,
	float* buffer, int bufferSize,
	const int* unitArray, int unitSize)
{
	//无需计算
	if (partSize <= 1) { return 1; }
	if (bufferSize <= 1) { return 1; }
	if (unitSize == 0) { return 1; }

	//计算合成单元最多用到的长度
	int maxUnitLength = 0;
	for (int i = 0; i < unitSize; i++) {
		if (unitArray[i] > maxUnitLength) { maxUnitLength = unitArray[i]; }
	}
	if (maxUnitLength > partSize) { return 1; }

	//滤波缓存
	float* partTemp1 = new float[maxUnitLength];
	float* partTemp2 = new float[maxUnitLength];

	//拷贝数据到缓存
	for (int i = 0; i < maxUnitLength; i++) {
		partTemp1[i] = part[i];
	}

	//合成每一片段
	int outDeviation = 0;
	for (int i = 0; i < unitSize; i++) {
		//进行滤波
		computeUnit(((i % 2) == 1) ? partTemp1 : partTemp2, ((i % 2) == 1) ? partTemp2 : partTemp1, maxUnitLength);

		//复制片段
		addUnit(
			&buffer[outDeviation], ((i % 2) == 1) ? partTemp1 : partTemp2,
			unitArray[i],
			1.f - (float)outDeviation / (float)(bufferSize - 1),
			1.f - (float)(outDeviation + unitArray[i] - 1) / (float)(bufferSize - 1));

		outDeviation += unitArray[i];
	}

	//清理缓存
	delete[] partTemp1;
	delete[] partTemp2;

	return 0;
}

void computeUnit(
	float* unitMemBase, const float* unitBase, int unitLength)
{
	constexpr float r = 0.5f;
	for (int i = 0; i < unitLength; i++) {
		if (i > 0) {
			unitMemBase[i] = unitBase[i] * r + unitBase[i - 1] * (1.f - r);
		}
		else {
			unitMemBase[i] = unitBase[i] * r + unitBase[unitLength - 1] * (1.f - r);
		}
	}
}

void addUnit(
	float* unitMemBase, const float* unitBase, int unitLength, float sCoefficient, float eCoefficient)
{
	for (int i = 0; i < unitLength; i++) {
		float coefficient = sCoefficient + (eCoefficient - sCoefficient) * (i / (float)(unitLength - 1));
		unitMemBase[i] += (unitBase[i] * coefficient);
	}
}