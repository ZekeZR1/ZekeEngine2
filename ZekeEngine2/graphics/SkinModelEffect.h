		#pragma once

#include "Shader.h"
/*!
*@brief	モデルエフェクト。
*/
class ModelEffect : public DirectX::IEffect {
protected:
	std::wstring m_materialName;	
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;
	Shader m_vsShader;
	Shader m_psShader;
	Shader m_vsShadowMap;			//シャドウマップ生成用の頂点シェーダー。
	Shader m_psShadowMap;		//シャドウマップ生成用のピクセルシェーダー。
	bool isSkining;
	ID3D11ShaderResourceView* m_albedoTex = nullptr;
	ID3D11ShaderResourceView* m_shadowMapSRV = nullptr;
	ID3D11ShaderResourceView* m_normalTexture = nullptr;
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;
	//std::array<ID3D11ShaderResourceView*, 4> m_albedoTextureStack = { nullptr };
	int m_albedoTextureStackPos = 0;
	EnRenderMode m_renderMode = enRenderMode_Invalid;	//レンダリングモード。

public:
	ModelEffect(const char* psmain, const char* vsmain)
	{
		m_psShader.Load("Assets/shader/model.fx", psmain, Shader::EnType::PS);
		m_pPSShader = &m_psShader;
		m_psShadowMap.Load("Assets/shader/model.fx", "PSMain_ShadowMap", Shader::EnType::PS);
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMain_ShadowMap", Shader::EnType::VS);
	}
	virtual ~ModelEffect()
	{
		if (m_albedoTex) {
			m_albedoTex->Release();
		}
		if (m_shadowMapSRV) {
			m_shadowMapSRV->Release();
		}
		if (m_normalTexture) {
			m_normalTexture->Release();
		}
		if (m_specularMapSRV) {
			m_shadowMapSRV->Release();
		}
	}
	void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;

	void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
	{
		*pShaderByteCode = m_vsShader.GetByteCode();
		*pByteCodeLength = m_vsShader.GetByteCodeSize();
	}
	void SetAlbedoTexture(ID3D11ShaderResourceView* tex)
	{
		m_albedoTex = tex;
	}
	void SetNormalTexture(ID3D11ShaderResourceView* tex) {
		m_normalTexture = tex;
	}
	void SetSpecularMap(ID3D11ShaderResourceView* tex) {
		m_specularMapSRV = tex;
	}
	void SetMatrialName(const wchar_t* matName)
	{
		m_materialName = matName;
	}
	bool EqualMaterialName(const wchar_t* name) const
	{
		return wcscmp(name, m_materialName.c_str()) == 0;
	}

	void SetRenderMode(EnRenderMode renderMode)
	{
		m_renderMode = renderMode;
	}
	void SetShadoMapSRV(ID3D11ShaderResourceView* srv)
	{
		m_shadowMapSRV = srv;
	}
};
/*!
*@brief
*  スキンなしモデルエフェクト。
*/
class NonSkinModelEffect : public ModelEffect {
public:
	NonSkinModelEffect(const char* psmain, const char* vsmain) : ModelEffect(psmain,vsmain)
	{
		m_vsShader.Load("Assets/shader/model.fx", vsmain, Shader::EnType::VS);
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMain_ShadowMap", Shader::EnType::VS);
		m_pVSShader = &m_vsShader;
		isSkining = false;
	}
	int n;
};
/*!
*@brief
*  スキンモデルエフェクト。
*/
class SkinModelEffect : public ModelEffect {
public:
	SkinModelEffect(const char* psmain, const char* vsmain) : ModelEffect(psmain,vsmain)
	{
		wchar_t hoge[256];
		GetCurrentDirectoryW(256, hoge);
		m_vsShader.Load("Assets/shader/model.fx", vsmain, Shader::EnType::VS);
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMainSkin_ShadowMap", Shader::EnType::VS);
		m_pVSShader = &m_vsShader;
		isSkining = true;
	}
};

/*!
*@brief
*  エフェクトファクトリ。
*/
class SkinModelEffectFactory : public DirectX::EffectFactory {
public:
	SkinModelEffectFactory(ID3D11Device* device, const char* psmain, const char* vsmain, const wchar_t* normalMap,const wchar_t* specularMap) :
		m_psmain(psmain),
		m_vsmain(vsmain),
		m_normalMapPath(normalMap),
		m_specularMapPath(specularMap),
		EffectFactory(device) {
	}
	std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info, ID3D11DeviceContext* deviceContext)override
	{
		OutputDebugStringW(info.name);
		std::shared_ptr<ModelEffect> effect;
		if (info.enableSkinning) {
			//スキニングあり。
			effect = std::make_shared<SkinModelEffect>(m_psmain,m_vsmain);
		}
		else {
			//スキニングなし。
			effect = std::make_shared<NonSkinModelEffect>(m_psmain, m_vsmain);
		}
		effect->SetMatrialName(info.name);
		if (info.diffuseTexture && *info.diffuseTexture)
		{
			ID3D11ShaderResourceView* texSRV;
			DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
			effect->SetAlbedoTexture(texSRV);
		}
		if (m_normalMapPath) {
			ID3D11ShaderResourceView* normalSRV;
			SetDirectory(L"Assets/modelData");
			DirectX::EffectFactory::CreateTexture(m_normalMapPath, deviceContext, &normalSRV);
			effect->SetNormalTexture(normalSRV);
		}
		if (m_specularMapPath) {
			ID3D11ShaderResourceView* specularSRV;
			SetDirectory(L"Assets/modelData");
			DirectX::EffectFactory::CreateTexture(m_specularMapPath, deviceContext, &specularSRV);
			effect->SetSpecularMap(specularSRV);
		}
		return effect;
	}

	void __cdecl CreateTexture(const wchar_t* name, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** textureView) override
	{
		return DirectX::EffectFactory::CreateTexture(name, deviceContext, textureView);
	}
	const char* m_psmain;
	const char* m_vsmain;
	const wchar_t* m_normalMapPath;
	const wchar_t* m_specularMapPath;
};
