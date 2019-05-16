#pragma once

#include "CShaderResource.h"
#include "Effect\CEffectEngine.h"
#include "RenderTarget.h"
#include "font/CFONT.h"

class CShaderResource;
class CEffectEngine;

enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_Normal,			//通常レンダリング。
	enRenderMode_Num,				//レンダリングモードの数。
};

class CGraphicsEngine
{
public:
	CGraphicsEngine();
	~CGraphicsEngine();

	bool InitDirectX(HWND hwnd);
	void Release();
	CShaderResource& GetShaderResources()
	{
		return m_shaderResources;
	}

	ID3D11Device* GetD3DDevice() {
		return m_pd3dDevice;
	}
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	DirectX::SpriteBatch* GetSpriteBatch() const
	{
		return m_spriteBatch.get();
	}
	DirectX::SpriteFont* GetSpriteFont(CFont::TextType type) const
	{
		switch (type) {
		case CFont::TextType::en_Alphabet:
			return m_spriteFont.get();
			break;
		case CFont::TextType::en_Japanese:
			return m_spriteFontJa.get();
			break;
		case CFont::TextType::en_JapaneseBIG:
			return m_spriteFontJaBig.get();
		}
	}

	Camera& GetMainCamera()
	{
		return m_mainCamera;
	}

	Camera& Get2DCamera()
	{
		return m_2dCamera;
	}

	int GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	int GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}
	int Get2DSpaceScreenWidth() const
	{
		return m_2dSpaceScreenWidth;
	}
	int Get2DSpaceScreenHeight() const
	{
		return m_2dSpaceScreenHeight;
	}
	/*!
	*@brief	エフェクトエンジンの取得。
	*/
	CEffectEngine& GetEffectEngine()
	{
		return m_effectEngine;
	}

	ID3D11RenderTargetView* GetTarget() {
		return m_backBuffer;
	}

	void BegineRender();
	void EndRender();

	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);

	void SetAmbientLight(float amb) {
		m_ambientLight = amb;
	}

	const float GetAmbientLight() {
		return m_ambientLight;
	}

private:
	CShaderResource m_shaderResources;
	ID3D11Device* m_pd3dDevice = NULL;
	ID3D11DeviceContext* m_pd3dDeviceContext = NULL;
	D3D_FEATURE_LEVEL m_featureLevel;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_backBuffer = NULL;
	ID3D11RasterizerState* m_rasterizerState = NULL;
	ID3D11Texture2D* m_depthStencil = NULL;
	ID3D11DepthStencilView* m_depthStencilView = NULL;
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFont;
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFontJa;
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFontJaBig;
	CEffectEngine m_effectEngine;
	Camera	m_mainCamera;		//3d camera
	Camera	m_2dCamera;
	int						m_2dSpaceScreenWidth = 1280;
	int						m_2dSpaceScreenHeight = 720;
	int						m_frameBufferWidth = 0;
	int						m_frameBufferHeight = 0;
	float						m_ambientLight = 0.5f;
};

