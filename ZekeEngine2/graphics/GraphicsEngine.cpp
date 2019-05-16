#include "stdafx.h"
#include "GraphicsEngine.h"
#include "ShaderResources.h"
#include "CShaderResource.h"


CGraphicsEngine::CGraphicsEngine()
{
}


CGraphicsEngine::~CGraphicsEngine()
{
	Release();
}

void CGraphicsEngine::BegineRender() {
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CGraphicsEngine::EndRender() {
	m_pSwapChain->Present(2, 0);
}

void CGraphicsEngine::Release()
{
	if (m_rasterizerState != NULL) {
		m_rasterizerState->Release();
		m_rasterizerState = NULL;
	}
	if (m_depthStencil != NULL) {
		m_depthStencil->Release();
		m_depthStencil = NULL;
	}
	if (m_depthStencilView != NULL) {
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
	if (m_backBuffer != NULL) {
		m_backBuffer->Release();
		m_backBuffer = NULL;
	}
	if (m_pSwapChain != NULL) {
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pd3dDeviceContext != NULL) {
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = NULL;
	}
	if (m_pd3dDevice != NULL) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}

bool CGraphicsEngine::InitDirectX(HWND hwnd) {
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;
	sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		sizeof(featureLevels) / sizeof(featureLevels[0]),
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pd3dDevice,
		&m_featureLevel,
		&m_pd3dDeviceContext
	);
	ID3D11Texture2D* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();
	{
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = (UINT)FRAME_BUFFER_W;
		texDesc.Height = (UINT)FRAME_BUFFER_H;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_depthStencil);
		//深度ステンシルビューを作成。
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = texDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		m_pd3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
	}
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	m_pd3dDevice->CreateRasterizerState(&desc, &m_rasterizerState);

	D3D11_VIEWPORT viewport;
	viewport.Width = FRAME_BUFFER_W;
	viewport.Height = FRAME_BUFFER_H;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_pd3dDeviceContext->RSSetViewports(1, &viewport);
	m_pd3dDeviceContext->RSSetState(m_rasterizerState);

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pd3dDeviceContext);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"Assets/font/myfile.spritefont");
	m_spriteFontJa = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"Assets/font/myfileJa.spritefont");
	//m_spriteFontJaBig = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"Assets/font/myfileJaBig.spritefont");

	m_effectEngine.Init();
	return true;
}


void CGraphicsEngine::ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//レンダリングターゲットの切り替え。
	m_pd3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
		m_pd3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void CGraphicsEngine::ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}
