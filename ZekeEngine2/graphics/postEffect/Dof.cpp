#include "stdafx.h"
//#include "Dof.h"
//
//
//Dof::~Dof()
//{
//	if (m_finalBlendState != nullptr) {
//		m_finalBlendState->Release();
//	}
//}
//void Dof::Init()
//{
//	auto mainRT = IGameObjectManager().GetMainRenderTarget();
//	m_bokeBlur.Init(mainRT->GetRenderTargetSRV(), 5.0);
//
//	m_vs.Load("Assets/shader/dof.fx", "VSMain", Shader::EnType::VS);
//	m_psFinal.Load("Assets/shader/dof.fx", "PSFinal", Shader::EnType::PS);
//
//	//�����������̃u�����h�X�e�[�g���쐬����B
//	CD3D11_DEFAULT defaultSettings;
//	//�f�t�H���g�Z�b�e�B���O�ŏ���������B
//	CD3D11_BLEND_DESC blendDesc(defaultSettings);
//	blendDesc.RenderTarget[0].BlendEnable = true;
//	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
//	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//	auto device = g_graphicsEngine->GetD3DDevice();
//
//	device->CreateBlendState(&blendDesc, &m_finalBlendState);
//}
///// <summary>
///// ���̊֐���PostEffect::Draw����Ă΂�Ă����B
///// </summary>
///// <param name="postEffect"></param>
//void Dof::Draw(PostEffect& postEffect)
//{
//	//���C�������_�����O�^�[�Q�b�g���擾�B
//	auto mainRT = IGameObjectManager().GetMainRenderTarget();
//	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
//
//	//�`��X�e�b�v_1
//	//�V�[���Ƀu���[�������Ă����B
//	{
//		m_bokeBlur.Execute(postEffect);
//	}
//	//�`��X�e�b�v_2
//	//�X�e�b�v�Q�Œ��o�����摜���A���C�������_�����O�^�[�Q�b�g��
//	//��������ł����B�{�P�Ȃ��ӏ��̓s�N�Z���L������B
//	{
//		g_graphicsEngine->ChangeRenderTarget(mainRT, mainRT->GetViewport());
//
//		//�{�P�摜�̃A�h���X��t0���W�X�^�ɐݒ肷��B
//		auto srv = m_bokeBlur.GetResultTextureSRV();
//		deviceContext->PSSetShaderResources(0, 1, &srv);
//		//�[�x�e�N�X�`����t1���W�X�^�ɐݒ肷��B
//		//auto depthTexSrv = g_game->GetDepthInViewRenderTarget()->GetRenderTargetSRV();
//		//TODO: get depth 
//		//auto depthTexSrv = IGameObject().GetDepthRenderTarget().GetRenderTargetSRV();
//		deviceContext->PSSetShaderResources(1, 1, &depthTexSrv);
//
//		ID3D11BlendState* oldBlendState;
//		float oldBlendFactor[4];
//		UINT oldMask;
//		deviceContext->OMGetBlendState(&oldBlendState, oldBlendFactor, &oldMask);
//		//�����������p�̃u�����f�B���O�X�e�[�g��ݒ肷��B
//		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
//		deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
//		//�t���X�N���[���`��B
//		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vs, m_psFinal);
//
//		//�u�����h�X�e�[�g��߂��B
//		deviceContext->OMSetBlendState(oldBlendState, oldBlendFactor, oldMask);
//		oldBlendState->Release();
//
//	}
//}