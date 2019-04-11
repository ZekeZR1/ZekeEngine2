#include "stdafx.h"
#include "CLight.h"
#include "Engine/graphics\CShader.h"

using namespace DirectX;

CLight::CLight()
{
	m_psShader.Load("Assets/shader/Light.fx", "ps_main", Shader::EnType::PS);
	m_vsShader.Load("Assets/shader/Light.fx", "vs_main",Shader::EnType::VS);
}

CLight::~CLight()
{
	//定数バッファを解放。
	if (m_cb != nullptr) {
		m_cb->Release();
	}

}


void CLight::Init() {
	//定数バッファを初期化。
	InitConstantBuffer();
}

void __cdecl CLight::Apply(ID3D11DeviceContext* deviceContext) 
{
	deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
	deviceContext->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);
}

void __cdecl CLight::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) 
{
	*pShaderByteCode = m_vsShader.GetByteCode();
	*pByteCodeLength = m_vsShader.GetByteCodeSize();
}



void CLight::Draw() {
	CommonStates state(g_graphicsEngine->GetD3DDevice());

	g_graphicsEngine->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m_cb);
	g_graphicsEngine->GetD3DDeviceContext()->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
	g_graphicsEngine->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, &m_psCb);
	g_graphicsEngine->GetD3DDeviceContext()->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);

	//単位マトリクス。
	CMatrix mWorld = CMatrix::Identity();
	//定数バッファを更新。
	ConstantBuffer vsCb;
	vsCb.world = mWorld;
	vsCb.projection = camera3d->GetProjectionMatrix();
	vsCb.view = camera3d->GetViewMatrix();
	g_graphicsEngine->GetD3DDeviceContext()->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//定数バッファをGPUに転送。
	g_graphicsEngine->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m_cb);
	//定数バッファを更新。
	lightSRV lightsrv;
	lightsrv.light = { 0.0f,500.0f,0.0f,0.0f };
	lightsrv.attenuation = { 0.f,0.f,0.f,0.f };
	g_graphicsEngine->GetD3DDeviceContext()->UpdateSubresource(m_psCb, 0, nullptr, &lightsrv, 0, 0);
	g_graphicsEngine->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, &m_psCb);
	//定数バッファをGPUに転送。
	g_graphicsEngine->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m_cb);
}

//定数バッファの初期化。
void CLight::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(ConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
																//作成。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//ピクセルシェーダー用の定数バッファを作成する。
	bufferDesc.ByteWidth = 256;		
	d3dDevice->CreateBuffer(&bufferDesc, NULL, &m_psCb);
}
