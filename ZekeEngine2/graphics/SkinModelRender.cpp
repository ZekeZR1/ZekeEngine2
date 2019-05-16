#include "stdafx.h"
#include "SkinModelRender.h"


SkinModelRender::SkinModelRender()
{
}


SkinModelRender::~SkinModelRender()
{
	IGameObjectManager().ClearACaster(&m_skinModel);
}


bool SkinModelRender::Start() {
	if (m_isShadowCaster) {
		IGameObjectManager().AddShadowCaster(&m_skinModel);
	}

	return true;
}

void SkinModelRender::Update() {
	m_animation.Update(IGameTime().GetFrameDeltaTime());
	m_skinModel.UpdateWorldMatrix(m_pos, m_rot, m_scale);
	m_animation.Update(IGameTime().GetFrameDeltaTime());
}

void SkinModelRender::Render() {
	m_skinModel.SetDirColor(m_dirCol,0);
	m_skinModel.SetDirLight(m_dirLight,0);
	m_skinModel.SetShadowMap(IGameObjectManager().GetShadowMap()->GetShadowMapSRV());
	//m_skinModel.Draw(enRenderMode_Normal,
	m_skinModel.Draw(enRenderMode_Normal,
		MainCamera().GetViewMatrix(),
		MainCamera().GetProjectionMatrix());
	m_skinModel.SetDirColor(m_defCol,0);
	m_skinModel.SetDirLight(m_defDir,0);
}


void SkinModelRender::Init(const wchar_t* filePath,
	AnimationClip* animationClips,
	int numAnimationClips,
	EnFbxUpAxis fbxUpAxis,
	const char* psmain,
	const char* vsmain,
	const wchar_t* normalMap,
	const wchar_t* specularMap
	){
	m_enFbxUpAxis = fbxUpAxis;
	m_psmain = psmain;
	m_vsmain = vsmain;

	m_skinModel.Init(filePath,m_enFbxUpAxis, m_psmain, m_vsmain, normalMap, specularMap);
	InitAnimation(animationClips, numAnimationClips);
}

void SkinModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips) {
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClips != nullptr) {
		m_animation.Init(m_skinModel, m_animationClips, m_numAnimationClips);
	}
}