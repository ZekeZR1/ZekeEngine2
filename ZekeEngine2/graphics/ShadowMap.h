#pragma once
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	CMatrix GetLighViewMatrix() const
	{
		return m_lightViewMatrix;
	}

	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
	}

	void ClearShadowCasters() {
		m_shadowCasters.clear();
	}

	void ClearACaster(SkinModel* casterModel) {
		m_shadowCasters.erase(remove(m_shadowCasters.begin(), m_shadowCasters.end(), casterModel), m_shadowCasters.end());
	}

	void UpdateFromLightTarget(CVector3 lightCameraPos, CVector3 lightCameraTarget);

	void UpdateFromLightDirection(CVector3 lightCameraPos, CVector3 lightDir);

	void RenderToShadowMap();

	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}

	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//ライトカメラの視点。
	CVector3 m_lightCameraTarget = CVector3::Zero();	//ライトカメラの注視点。
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//ライトビュー行列。
	CMatrix m_lightProjMatrix = CMatrix::Identity();	//ライトプロジェクション行列。
	RenderTarget m_shadowMapRT;						//シャドウマップを描画するレンダリングターゲット。
	std::vector< SkinModel*> m_shadowCasters;	//シャドウキャスターの配列。
};

