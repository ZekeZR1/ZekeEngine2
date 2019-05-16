#include "stdafx.h"
#include "Bloom.h"
#include "PostEffect.h"

Bloom::Bloom()
{
	InitShader();
	InitRenderTarget();
	InitAlphaBlendState();
	InitConstantBuffer();
	InitSamplerState();
}


Bloom::~Bloom()
{
	if (m_disableBlendState != nullptr) {
		m_disableBlendState->Release();
	}
	if (m_blurParamCB != nullptr) {
		m_blurParamCB->Release();
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
	if (m_finalBlendState != nullptr) {
		m_finalBlendState->Release();
	}
}
void Bloom::InitConstantBuffer()
{
	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = (((sizeof(SBlurParam) - 1) / 16) + 1) * 16;	//16バイトアライメントに切りあげる。
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	GraphicsEngine().GetD3DDevice()->CreateBuffer(&desc, NULL, &m_blurParamCB);
}
void Bloom::InitSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	GraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void Bloom::InitShader()
{
	m_vsShader.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_psLuminance.Load("Assets/shader/bloom.fx", "PSSamplingLuminance", Shader::EnType::PS);
	m_vsXBlur.Load("Assets/shader/bloom.fx", "VSXBlur", Shader::EnType::VS);
	m_vsYBlur.Load("Assets/shader/bloom.fx", "VSYBlur", Shader::EnType::VS);
	m_psBlur.Load("Assets/shader/bloom.fx", "PSBlur", Shader::EnType::PS);
	m_psCombine.Load("Assets/shader/bloom.fx", "PSCombine", Shader::EnType::PS);
	m_copyVS.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);
	m_copyPS.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);
}
void Bloom::InitRenderTarget()
{
	int w = FRAME_BUFFER_W;
	int h = FRAME_BUFFER_H;
	//輝度抽出用のレンダリングターゲットを作成。
	DXGI_SAMPLE_DESC multiSampleDesc;
	ZeroMemory(&multiSampleDesc, sizeof(multiSampleDesc));
	multiSampleDesc.Count = 1;
	multiSampleDesc.Quality = 0;
	m_luminanceRT.Create(
		w,
		h,
		DXGI_FORMAT_R16G16B16A16_FLOAT
		);
	//ダウンサンプリング用のレンダリングターゲットを作成する。
	for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
		int shift = i + 1;
		int baseIndex = i * 2;
		//横ブラー用。
		m_downSamplingRT[baseIndex].Create(
			w >> shift,
			h >> (shift - 1),
			DXGI_FORMAT_R16G16B16A16_FLOAT
		);
		//縦ブラー用。
		m_downSamplingRT[baseIndex + 1].Create(
			w >> shift,
			h >> shift,
			DXGI_FORMAT_R16G16B16A16_FLOAT
		);
		}
	
	//ぼかし合成用。
	m_combineRT.Create(
		w >> 2,
		h >> 2,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
}
void Bloom::InitAlphaBlendState()
{
	CD3D11_DEFAULT defaultSettings;
	//デフォルトセッティングで初期化する。
	CD3D11_BLEND_DESC blendDesc(defaultSettings);
	auto device = GraphicsEngine().GetD3DDevice();

	device->CreateBlendState(&blendDesc, &m_disableBlendState);

	//最終合成用のブレンドステートを作成する。
	//最終合成は加算合成。
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	device->CreateBlendState(&blendDesc, &m_finalBlendState);
}
void Bloom::Update()
{
	float total = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_blurParam.weights[i] = expf(-0.5f*(float)(i*i) / m_blurDispersion);
		total += 2.0f*m_blurParam.weights[i];

	}
	// 規格化
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_blurParam.weights[i] /= total;
	}
}

void Bloom::Draw(PostEffect& postEffect)
{
	auto deviceContext = GraphicsEngine().GetD3DDeviceContext();
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//まずは輝度を抽出する。
	{
		//αブレンドを無効にする。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
		//輝度抽出用のレンダリングターゲットに変更する。
		GraphicsEngine().ChangeRenderTarget(&m_luminanceRT, m_luminanceRT.GetViewport());
		//レンダリングターゲットのクリア。
		m_luminanceRT.ClearRenderTarget(clearColor);
			//シーンをテクスチャとする。
		auto mainRTTexSRV = IGameObjectManager().GetMainRenderTarget()->GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &mainRTTexSRV);

		//フルスクリーン描画。
		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vsShader, m_psLuminance);
	}
	//輝度をぼかす。
	{
		RenderTarget* prevRt = &m_luminanceRT;
		int rtIndex = 0;
		for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
			//XBlur
			{
				//	//Xブラー用のレンダリングターゲットに変更する。
				GraphicsEngine().ChangeRenderTarget(&m_downSamplingRT[rtIndex], m_downSamplingRT[rtIndex].GetViewport());
				m_blurParam.offset.x = 16.0f / prevRt->GetWidth();
				m_blurParam.offset.y = 0.0f;
				deviceContext->UpdateSubresource(m_blurParamCB, 0, nullptr, &m_blurParam, 0, 0);
				m_downSamplingRT[rtIndex].ClearRenderTarget(clearColor);
				//rc.ClearRenderTargetView(0, clearColor);
				auto prevSRV = prevRt->GetRenderTargetSRV();
				deviceContext->VSSetShaderResources(0, 1, &prevSRV);
				deviceContext->PSSetShaderResources(0, 1, &prevSRV);
				deviceContext->PSSetConstantBuffers(0, 1, &m_blurParamCB);
				postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vsXBlur, m_psBlur);
			}
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;
			//YBlur
			{
				GraphicsEngine().ChangeRenderTarget(&m_downSamplingRT[rtIndex], m_downSamplingRT[rtIndex].GetViewport());
				m_blurParam.offset.x = 0.0f;
				//TODO : add some func to bloom shader
				m_blurParam.offset.y = 16.0f / prevRt->GetWidth();
				//
				auto preSRV = prevRt->GetRenderTargetSRV();
				deviceContext->VSSetShaderResources(0, 1, &preSRV);
				deviceContext->PSSetShaderResources(0, 1, &preSRV);
				m_downSamplingRT[rtIndex].ClearRenderTarget(clearColor);
				deviceContext->UpdateSubresource(m_blurParamCB, 0, nullptr, &m_blurParam, 0, 0);
				//ブラー用の定数バッファを設定する。
				deviceContext->PSSetConstantBuffers(0, 1, &m_blurParamCB);
				postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vsYBlur, m_psBlur);
			}
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;
		}
	}
	//ボケ画像の作成。
	{
		GraphicsEngine().ChangeRenderTarget(&m_combineRT, m_combineRT.GetViewport());
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
		m_combineRT.ClearRenderTarget(clearColor);
		auto srv3 = m_downSamplingRT[3].GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &srv3);
		auto srv5 = m_downSamplingRT[3].GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(1, 1, &srv5);
		auto srv7 = m_downSamplingRT[3].GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(2, 1, &srv7);
		auto srv9 = m_downSamplingRT[3].GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(3, 1, &srv9);
		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vsShader, m_psCombine);
	}
	//最終合成。
	{
		auto mainRT = IGameObjectManager().GetMainRenderTarget();
		GraphicsEngine().ChangeRenderTarget(mainRT, mainRT->GetViewport());
		// アルファブレンディングを加算合成にする。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
		//定数バッファを設定する。
		auto combineSRV = m_combineRT.GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &combineSRV);
		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_copyVS, m_copyPS);
		//アルファブレンディングをもとに戻す。
		//rc.OMSetBlendState(AlphaBlendState::trans, 0, 0xFFFFFFFF);
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);

	}
}
	