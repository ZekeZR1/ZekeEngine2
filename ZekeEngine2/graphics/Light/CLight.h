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
	ID3D11Buffer*		m_cb = nullptr;						//!<�萔�o�b�t�@�B
	ID3D11Buffer*		m_psCb = nullptr;					//!<�s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
	//ID3D11ShaderResourceView* m_grayTextureSRV = nullptr;	//!<�O���[�e�N�X�`����SRV
	struct ConstantBuffer {
		CMatrix world;
		CMatrix view;
		CMatrix projection;
		//CVector4   light;
		//CVector4   attenuation;
	};
	struct lightSRV {
		//�������W
		//���������p�����[�^
		CVector4   light;
		CVector4   attenuation;
	};
};
