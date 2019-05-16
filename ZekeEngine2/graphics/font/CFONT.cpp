#include "stdafx.h"
#include "CFONT.h"
CFont::CFont()
{
	m_spriteBatch = GraphicsEngine().GetSpriteBatch();
	m_spriteFont = GraphicsEngine().GetSpriteFont(m_type);
	m_scaleMat.MakeScaling(
		{
			GraphicsEngine().GetFrameBufferWidth() / static_cast<float>(GraphicsEngine().Get2DSpaceScreenWidth()),
			GraphicsEngine().GetFrameBufferHeight() / static_cast<float>(GraphicsEngine().Get2DSpaceScreenHeight()),
			1.0f
		}
	);
}
CFont::~CFont()
{
}

void CFont::Begin()
{
	auto dc = GraphicsEngine().GetD3DDeviceContext();
	dc->OMGetBlendState(&m_blendState, m_BlendFactor, &m_SampleMask);
	dc->RSGetState(&m_rasterizerState);
	dc->OMGetDepthStencilState(&m_depthStencilState, &m_StencilRef);
	m_spriteBatch->Begin();
	m_spriteFont = GraphicsEngine().GetSpriteFont(m_type);
	/*if (m_type == en_Japanese)
		m_spriteFont = g_graphicsEngine->GetSpriteFont(en_Japanese);*/
}
void CFont::End()
{
	m_spriteBatch->End();
	float blendFactor[4] = { 0.0f };
	auto dc = GraphicsEngine().GetD3DDeviceContext();
	dc->OMSetBlendState(m_blendState,m_BlendFactor,m_SampleMask);
	dc->RSSetState(m_rasterizerState);
	dc->OMSetDepthStencilState(m_depthStencilState, m_StencilRef);
	if (m_blendState != nullptr) {
		m_blendState->Release();
	}
}
void CFont::Draw(
	wchar_t const* text,
	const CVector2& position,
	const CVector4& color,
	float rotation,
	float scale,
	CVector2 pivot)
{
	CVector2 pos = position;
	float frameBufferHalfWidth = GraphicsEngine().Get2DSpaceScreenWidth() * 0.5f;
	float frameBufferHalfHeight = GraphicsEngine().Get2DSpaceScreenHeight() * 0.5f;
	pos.x += frameBufferHalfWidth;
	pos.y = -pos.y + frameBufferHalfHeight;

	m_spriteFont->DrawString(
		m_spriteBatch,
		text,
		pos.vec,
		color,
		rotation,
		DirectX::XMFLOAT2(pivot.x, pivot.y),
		scale
	);
}