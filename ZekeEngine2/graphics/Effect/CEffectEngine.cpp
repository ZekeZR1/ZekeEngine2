#include "stdafx.h"
#include "CEffectEngine.h"

CEffectEngine::CEffectEngine()
{
}


CEffectEngine::~CEffectEngine()
{
	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	m_manager->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	m_renderer->Destroy();
}


void CEffectEngine::Init() {
	m_renderer = ::EffekseerRendererDX11::Renderer::Create(GraphicsEngine().GetD3DDevice(), GraphicsEngine().GetD3DDeviceContext(), 2000);
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_manager = ::Effekseer::Manager::Create(2000);
	m_manager->SetCoordinateSystem(::Effekseer::CoordinateSystem::LH);
	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
}

void CEffectEngine::Update() {
	// ���_�ʒu���m��
	CVector3 pos = MainCamera().GetPosition();
	m_position = ::Effekseer::Vector3D(pos.x, pos.y, pos.z);

	// �J�����s���ݒ�
	CVector3 tar = MainCamera().GetTarget();
	/*
	g_renderer->SetCameraMatrix(
	::Effekseer::Matrix44().LookAtLH(g_position, ::Effekseer::Vector3D(tar.x, tar.y, tar.z), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
	*/
	m_renderer->SetCameraMatrix(MainCamera().GetViewMatrix());

	// ���e�s���ݒ�
	/*
	g_renderer->SetProjectionMatrix(
	::Effekseer::Matrix44().PerspectiveFovLH(camera3d->GetViewAngle(), camera3d->GetAspect(), 0.1f, 50000.0f));
	*/
	m_renderer->SetProjectionMatrix(MainCamera().GetProjectionMatrix());

	m_manager->Update();
}


void CEffectEngine::Draw() {
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();
}

