#pragma once
#include "../graphics/GPUBuffer/CGPUBuffer.h"
class CConstantBuffer : public CGPUBuffer {
public:
	CConstantBuffer();
	~CConstantBuffer();
	bool Create(const void* pInitData, int bufferSize);
};

