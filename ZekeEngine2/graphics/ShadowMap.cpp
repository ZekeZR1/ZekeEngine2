#include "stdafx.h"
#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
	m_shadowMapRT.Create(
		2048,
		2048,
		DXGI_FORMAT_R32_FLOAT
	);
}

ShadowMap::~ShadowMap()
{
	m_shadowMapRT.ReleaseRenderTarget();
}

void ShadowMap::UpdateFromLightDirection(CVector3 lightCameraPos, CVector3 lightDir)
{
	CVector3 lightCameraUpAxis;
	if (fabsf(lightDir.y) > 0.99998f) {
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		lightCameraUpAxis = CVector3::AxisY();
	}
	m_lightViewMatrix.MakeLookAt(
		m_lightCameraPosition,
		m_lightCameraTarget,
		lightCameraUpAxis
	);

	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		3000,
		3000,
		0.1f,
		5000.0f
	);
}
void ShadowMap::UpdateFromLightTarget(CVector3 lightCameraPos, CVector3 lightCameraTarget)
{
	m_lightCameraTarget = lightCameraTarget;
	m_lightCameraPosition = lightCameraPos;
	auto lightDir = m_lightCameraTarget - m_lightCameraPosition;
	if (lightDir.Length() < 0.0001f) {
		std::abort();
	}
	lightDir.Normalize();
	UpdateFromLightDirection(lightCameraPos, lightDir);
}
void ShadowMap::RenderToShadowMap()
{
	auto d3dDeviceContext = GraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲット切り替え
	ID3D11RenderTargetView* rts[] = {
		m_shadowMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT.GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT.ClearRenderTarget(clearColor);

	//シャドウキャスターをシャドウマップにレンダリング。
	for (auto caster : m_shadowCasters) {
		caster->Draw(
			enRenderMode_CreateShadowMap,
			m_lightViewMatrix,
			m_lightProjMatrix
		);
	}
}