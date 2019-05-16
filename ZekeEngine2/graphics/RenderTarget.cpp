#include "stdafx.h"
#include "RenderTarget.h"


RenderTarget::RenderTarget()
{
}


RenderTarget::~RenderTarget()
{
}

void RenderTarget::Create(unsigned int w, unsigned int h, DXGI_FORMAT texFormat) {
	m_width = (float)w;
	m_height = (float)h;
	m_format = texFormat;
	auto d3dDevice = GraphicsEngine().GetD3DDevice();
	//create rendering target texture
	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	{
		texDesc.Width = w;
		texDesc.Height = h;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = texFormat;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		d3dDevice->CreateTexture2D(&texDesc, nullptr, &m_renderTargetTex);
	}
	//create rendering target view
	{
		D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = 0;
		d3dDevice->CreateRenderTargetView(m_renderTargetTex, &viewDesc, &m_renderTargetView);
	}
	//create shader resource view
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		d3dDevice->CreateShaderResourceView(m_renderTargetTex, &srvDesc, &m_renderTargetSRV);
	}
	//create depth Stencil texture
	D3D11_TEXTURE2D_DESC depthTexDesc = texDesc;

	{
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
		d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &m_depthStencilTex);
	}
	//create depth stencil view
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = depthTexDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		d3dDevice->CreateDepthStencilView(m_depthStencilTex, &depthStencilViewDesc, &m_depthStencilView);
	}
	{
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = w;
		m_viewport.Height = h;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
	}
}

void RenderTarget::ReleaseRenderTarget() {
	if (m_depthStencilView != nullptr) {
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
	if (m_depthStencilTex != nullptr) {
		m_depthStencilTex->Release();
		m_depthStencilTex = nullptr;
	}

	if (m_renderTargetView != nullptr) {
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_renderTargetTex != nullptr) {
		m_renderTargetTex->Release();
		m_renderTargetTex = nullptr;
	}
	if (m_renderTargetSRV != nullptr) {
		m_renderTargetSRV->Release();
	}
}

void RenderTarget::ClearRenderTarget(float* clearColor)
{
	auto d3dDeviceContext = GraphicsEngine().GetD3DDeviceContext();
	d3dDeviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	d3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

