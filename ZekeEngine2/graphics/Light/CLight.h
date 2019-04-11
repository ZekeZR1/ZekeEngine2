#pragma once
#include "Engine/graphics/CShader.h"
#include "Engine/graphics/Shader.h"

using namespace DirectX;

class CLight
{
public:
	CLight();
	~CLight();
	void Draw();
	void Init();
private:
	void InitSamplerState();
	void InitConstantBuffer();
	void __cdecl Apply(ID3D11DeviceContext*);
	void __cdecl GetVertexShaderBytecode(void const**, size_t*);
	Shader m_vsShader;
	Shader m_psShader;
	ID3D11Buffer*		m_cb = nullptr;						//!<定数バッファ。
	ID3D11Buffer*		m_psCb = nullptr;					//!<ピクセルシェーダー用の定数バッファ。
	//ID3D11ShaderResourceView* m_grayTextureSRV = nullptr;	//!<グレーテクスチャのSRV
	struct ConstantBuffer {
		CMatrix world;
		CMatrix view;
		CMatrix projection;
		//CVector4   light;
		//CVector4   attenuation;
	};
	struct lightSRV {
		//光源座標
		//光源減衰パラメータ
		CVector4   light;
		CVector4   attenuation;
	};
};
