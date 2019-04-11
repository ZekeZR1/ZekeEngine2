#pragma once
class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	//tex weight, text hight
	void Create(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

	void ReleaseRenderTarget();

	void ClearRenderTarget(float* clearColor);

	ID3D11RenderTargetView* GetRenderTargetView() {
		return m_renderTargetView;
	}

	ID3D11DepthStencilView* GetDepthStensilView() {
		return m_depthStencilView;
	}

	ID3D11ShaderResourceView* GetRenderTargetSRV() {
		return m_renderTargetSRV;
	}

	D3D11_VIEWPORT* GetViewport()
	{
		return &m_viewport;
	}

	float GetWidth() const
	{
		return m_width;
	}

	float GetHeight() const
	{
		return m_height;
	}

	DXGI_FORMAT GetTextureFormat() const {
		return m_format;
	}
private:
	DXGI_FORMAT m_format;
	float m_width = 0;										//!<レンダリングターゲットの幅。
	float m_height = 0;										//!<レンダリングターゲットの高さ。
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTex = nullptr;
	ID3D11ShaderResourceView* m_renderTargetSRV = nullptr;
	ID3D11Texture2D* m_renderTargetTex = nullptr;
	D3D11_VIEWPORT				m_viewport;						
};

