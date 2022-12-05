#include "../KarPlusStrong.cuh"
#include "../../algorithm/KarPlusStrong.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <time.h>
#include <numeric>

inline int16_t PCMFloat32ToInt16FmtConvert(float sample)
{
	if (sample < -0.999999f) {
		return INT16_MIN;
	}
	else if (sample > 0.999999f) {
		return INT16_MAX;
	}
	else {
		return static_cast<int16_t>(sample * 32767.0f);
	}
}

inline float PCMInt16ToFloat32FmtConvert(int16_t sample)
{
	return static_cast<float>(sample) / -static_cast<float>(INT16_MIN);
}

int main()
{
	std::cout << "选择要加载的合成单元数据：";
	std::flush(std::cout);

	std::string unitPath;
	std::cin >> unitPath;

	FILE* unitFile = fopen(unitPath.c_str(), "r");
	if (!unitFile) {
		std::cout << "未能打开文件：" << unitPath << std::endl;
		system("pause");
		return -1;
	}

	fseek(unitFile, 0, SEEK_END);
	auto unitSize = ftell(unitFile);
	fseek(unitFile, 0, SEEK_SET);

	std::ifstream unitInputStream(unitFile);
	if (!unitInputStream.good()) {
		std::cout << "未能建立文件流：" << unitPath << std::endl;
		fclose(unitFile);
		system("pause");
		return -1;
	}

	if (unitSize > 0) {
		std::cout << "单元文件大小：" << unitSize << std::endl;
	}
	else {
		std::cout << "单元文件大小为零！" << std::endl;
		fclose(unitFile);
		system("pause");
		return -1;
	}

	int unitNum = unitSize / sizeof(int16_t);
	std::cout << "单元采样数：" << unitNum << std::endl;

	std::vector<float> unitList;
	unitList.resize(unitNum);

	for (int i = 0; i < unitNum; i++) {
		int16_t unit = 0;
		unitInputStream.read((char*)&unit, sizeof(int16_t));
		unitList[i] = PCMInt16ToFloat32FmtConvert(unit);
	}

	fclose(unitFile);

	float unitMean= std::accumulate(unitList.begin(), unitList.end(), 0.f) / unitList.size();
	for (int i = 0; i < unitNum; i++) {
		unitList[i] -= unitMean;
	}
	std::cout << "已加载合成单元！" << std::endl;

	int sampleRate = 48000;
	std::cout << "采样率：" << sampleRate << std::endl;

	std::cout << "输入合成频率：";
	std::flush(std::cout);
	float fre = 440.00;
	std::cin >> fre;

	std::cout << "输入合成时长：";
	std::flush(std::cout);
	float time = 10.0;
	std::cin >> time;

	int sampleSize = time * sampleRate;
	int partSize = 1 / fre * sampleRate;
	int partNum = std::ceil(sampleSize / (double)partSize);
	int lastPartSize = sampleSize % partSize;

	std::cout << "采样数：" << sampleSize << std::endl;
	std::cout << "片段数：" << partNum << std::endl;
	std::cout << "单个片段采样数：" << partSize << std::endl;
	std::cout << "末尾片段采样数：" << lastPartSize << std::endl;

	std::vector<int> partList;
	partList.resize(partNum);
	for (int i = 0; i < partNum; i++) {
		partList[i] = partSize;
	}
	partList[partNum - 1] = lastPartSize;

	std::vector<float> gpuBuffer;
	gpuBuffer.resize(sampleSize);
	std::vector<float> cpuBuffer;
	cpuBuffer.resize(sampleSize);

#if CUDA_ENABLED
	std::cout << "开始GPU合成！" << std::endl;
	clock_t gpuStartTime = clock();

	if (auto result = doSynthesisGPU(unitList.data(), unitSize, gpuBuffer.data(), sampleSize, partList.data(), partNum)) {
		std::cout << "GPU合成失败, 返回：" << (int)result << std::endl;
		system("pause");
		return -1;
	}

	clock_t gpuEndTime = clock();
	std::cout << "GPU合成结束, 用时：" << (gpuEndTime - gpuStartTime) / (float)CLOCKS_PER_SEC << std::endl;
#else
	std::cout << "不支持GPU合成！" << std::endl;
#endif // CUDA_ENABLED

	std::cout << "开始CPU合成！" << std::endl;
	clock_t cpuStartTime = clock();

	if (auto result = doSynthesisCPU(unitList.data(), unitSize, cpuBuffer.data(), sampleSize, partList.data(), partNum)) {
		std::cout << "CPU合成失败, 返回：" << (int)result << std::endl;
		system("pause");
		return -1;
	}

	clock_t cpuEndTime = clock();
	std::cout << "CPU合成结束, 用时：" << (cpuEndTime - cpuStartTime) / (float)CLOCKS_PER_SEC << std::endl;

	for (int i = sampleSize - lastPartSize; i < sampleSize; i++) {
		if (cpuBuffer[i] > 0.1) {
			std::cout << i << ":" << cpuBuffer[i] << std::endl;
			system("pause");
		}
	}

	std::cout << "输入GPU合成结果储存位置：";
	std::flush(std::cout);

	std::string gpuOutPath;
	std::cin >> gpuOutPath;

	FILE* gpuOutFile = fopen(gpuOutPath.c_str(), "w");
	if (!gpuOutFile) {
		std::cout << "未能打开文件：" << gpuOutPath << std::endl;
		system("pause");
		return -1;
	}

	std::ofstream gpuOutStream(gpuOutFile);
	if (!gpuOutStream.good()) {
		std::cout << "未能建立文件流：" << gpuOutPath << std::endl;
		fclose(gpuOutFile);
		system("pause");
		return -1;
	}

	for (int i = 0; i < sampleSize; i++) {
		int16_t temp = PCMFloat32ToInt16FmtConvert(gpuBuffer[i]);
		gpuOutStream.write((const char*)&temp, sizeof(int16_t));
	}

	fclose(gpuOutFile);
	std::cout << "已保存GPU合成结果！" << std::endl;

	std::cout << "输入CPU合成结果储存位置：";
	std::flush(std::cout);

	std::string cpuOutPath;
	std::cin >> cpuOutPath;

	FILE* cpuOutFile = fopen(cpuOutPath.c_str(), "w");
	if (!cpuOutFile) {
		std::cout << "未能打开文件：" << cpuOutPath << std::endl;
		system("pause");
		return -1;
	}

	std::ofstream cpuOutStream(cpuOutFile);
	if (!cpuOutStream.good()) {
		std::cout << "未能建立文件流：" << cpuOutPath << std::endl;
		fclose(cpuOutFile);
		system("pause");
		return -1;
	}

	for (int i = 0; i < sampleSize; i++) {
		int16_t temp = PCMFloat32ToInt16FmtConvert(cpuBuffer[i]);
		cpuOutStream.write((const char*)&temp, sizeof(int16_t));
	}

	fclose(cpuOutFile);
	std::cout << "已保存CPU合成结果！" << std::endl;

	system("pause");
	return 0;
}
