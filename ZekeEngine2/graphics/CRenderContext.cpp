#include "stdafx.h"
#include "../graphics/CRenderContext.h"
#include "../graphics/RenderTarget.h"
#include "../graphics/ShaderResouceView.h"
#include "../graphics/CUnorderedAccessView.h"
#include "../graphics/CVertexBuffer.h"
#include "../graphics/CConstantBuffer.h"
#include "../graphics/GPUBuffer/CGPUBuffer.h"
#include "../graphics/CShader.h"
#include "../graphics/CSamplerState.h"

CRenderContext::CRenderContext()
{
}


CRenderContext::~CRenderContext()
{
}


void CRenderContext::Init(ID3D11DeviceContext* pD3DDeviceContext)
{
	//TK_ASSERT(pD3DDeviceContext != nullptr, "pD3DDeviceContextÇ™NULLÇ≈Ç∑ÅBçÏê¨ÇµÇƒÇ©ÇÁåƒÇÒÇ≈ÇÀÅB");
	m_pD3DDeviceContext = pD3DDeviceContext;
}
void CRenderContext::OMSetRenderTargets(unsigned int NumViews, RenderTarget* renderTarget[])
{
	//TK_ASSERT(NumViews <= MRT_MAX, "NumViews is invalid");

	ZeroMemory(m_renderTargetViews, sizeof(m_renderTargetViews));
	memcpy(m_renderTargetViews, renderTarget, sizeof(RenderTarget*) * NumViews);

	ID3D11RenderTargetView* renderTargetViews[MRT_MAX] = { nullptr };
	ID3D11DepthStencilView*	depthStencilView = nullptr;

	if (renderTarget != nullptr) {
		depthStencilView = m_renderTargetViews[0]->GetDepthStensilView();
		for (unsigned int i = 0; i < NumViews; i++) {
			renderTargetViews[i] = m_renderTargetViews[i]->GetRenderTargetView();
		}
	}
	m_pD3DDeviceContext->OMSetRenderTargets(NumViews, renderTargetViews, depthStencilView);
	m_numRenderTargetView = NumViews;
}
