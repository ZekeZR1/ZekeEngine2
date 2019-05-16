#include "stdafx.h"
#include "CSamplerState.h"



CSamplerState::CSamplerState()
{
}


CSamplerState::~CSamplerState()
{
	Release();
}

bool CSamplerState::Create(const D3D11_SAMPLER_DESC& desc)
{
	Release();
	HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
	if (FAILED(hr)) {
		//TK_LOG("Failed CSamplerState::Create\n");
		return false;
	}
	return true;
}