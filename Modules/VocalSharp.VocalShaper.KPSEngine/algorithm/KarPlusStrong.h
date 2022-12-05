#pragma once

//调用此函数进行合成
//part: 用于合成的原始单元
//buffer: 目标存储位置
//unitArray: 合成单元序列中每一单元的目标长度
int doSynthesisCPU(
	const float* part, int partSize,
	float* buffer, int bufferSize,
	const int* unitArray, int unitSize);
