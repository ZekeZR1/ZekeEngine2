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
	//�萔�o�b�t�@������B
	if (m_cb != nullptr) {
		m_cb->Release();
	}

}


void CLight::Init() {
	//�萔�o�b�t�@���������B
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

	//�P�ʃ}�g���N�X�B
	CMatrix mWorld = CMatrix::Identity();
	//�萔�o�b�t�@���X�V�B
	ConstantBuffer vsCb;
	vsCb.world = mWorld;
	vsCb.projection = camera3d->GetProjectionMatrix();
	vsCb.view = camera3d->GetViewMatrix();
	g_graphicsEngine->GetD3DDeviceContext()->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//�萔�o�b�t�@��GPU�ɓ]���B
	g_graphicsEngine->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m_cb);
	//�萔�o�b�t�@���X�V�B
	lightSRV lightsrv;
	lightsrv.light = { 0.0f,500.0f,0.0f,0.0f };
	lightsrv.attenuation = { 0.f,0.f,0.f,0.f };
	g_graphicsEngine->GetD3DDeviceContext()->UpdateSubresource(m_psCb, 0, nullptr, &lightsrv, 0, 0);
	g_graphicsEngine->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, &m_psCb);
	//�萔�o�b�t�@��GPU�ɓ]���B
	g_graphicsEngine->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m_cb);
}

//�萔�o�b�t�@�̏������B
void CLight::InitConstantBuffer()
{
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(ConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
																//�쐬�B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//�s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���쐬����B
	bufferDesc.ByteWidth = 256;		
	d3dDevice->CreateBuffer(&bufferDesc, NULL, &m_psCb);
}
