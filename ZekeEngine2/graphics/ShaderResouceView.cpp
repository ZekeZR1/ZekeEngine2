#include "stdafx.h"
#include "ShaderResouceView.h"
#include "Noncopyable.h"
#include "StructedBuffer.h"


ShaderResouceView::ShaderResouceView()
{
}


ShaderResouceView::~ShaderResouceView()
{
}

void ShaderResouceView::Release()
{
	if (m_srv != nullptr) {
		m_srv->Release();
		m_srv = nullptr;
	}
	m_isValid = false;
}
bool ShaderResouceView::Create(StructedBuffer& structuredBuffer)
{
	Release();
	ID3D11Buffer* pBuf = structuredBuffer.GetBody();
	if (pBuf != nullptr) {
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		pBuf->GetDesc(&descBuf);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		desc.BufferEx.FirstElement = 0;

		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

		HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateShaderResourceView(pBuf, &desc, &m_srv);
		if (FAILED(hr)) {
			return false;
		}
	}
	//有効になった印
	m_isValid = true;
	return true;
}
/*!
*@brief	テクスチャ用のSRVを作成。
*@param[in]	texture		テクスチャ。
*/
bool ShaderResouceView::Create(ID3D11Texture2D* texture)
{
	Release();
	if (texture != nullptr) {
		D3D11_TEXTURE2D_DESC texDesc;
		texture->GetDesc(&texDesc);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = texDesc.Format;
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;

		HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateShaderResourceView(texture, &SRVDesc, &m_srv);
		if (FAILED(hr)) {
			return false;
		}
	}
	//有効になった印。
	m_isValid = true;
	return true;
}
/*!
*@brief	DDSファイルからテクスチャ用のSRVを作成。
*@param[in]	fileName		ファイル名。
*/
bool ShaderResouceView::CreateFromDDSTextureFromFile(const wchar_t* fileName)
{
	Release();
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), fileName, 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_srv);
	if (FAILED(hr)) {
		//TK_WARNING_MESSAGE_BOX("Failed create texture");
		return false;
	}
	return true;
}