#include "stdafx.h"
#include "CEffect.h"

CEffect::CEffect() {

}

CEffect::~CEffect() {
	ES_SAFE_RELEASE(m_effect);
}


bool CEffect::Start() {
	return true;
}

void CEffect::Update() {
	GraphicsEngine().GetEffectEngine().GetEffekseerManager().SetScale(m_handle, m_scale.x, m_scale.y, m_scale.z);
	Effekseer::Matrix43 mBase;
	CMatrix mRot, mScale, mTrans;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	mScale.MakeScaling(m_scale);
	Effekseer::Matrix43::Multiple(mBase, mScale, mRot);
	Effekseer::Matrix43::Multiple(mBase, mBase, mTrans);
	GraphicsEngine().GetEffectEngine().GetEffekseerManager().SetBaseMatrix(m_handle, mBase);
	//CMatrix mTrans, mRot, mScale, mBase;
	//mTrans.MakeTranslation(m_position);
	//mRot.MakeRotationFromQuaternion(m_rotation);
	//mScale.MakeScaling(m_scale);
	//mBase = mBase * mTrans;
	//mBase = mScale * mRot;
	//g_graphicsEngine->GetEffectEngine().GetEffekseerManager().SetBaseMatrix(m_handle, mBase);
	if (!IsPlay() && isPlayed) {
		Stop();
		DeleteGO(this);
	}
}

void CEffect::Render() {
}


void CEffect::Play(const wchar_t* filepath, float speed) {
	m_effect = Effekseer::Effect::Create(&(GraphicsEngine().GetEffectEngine().GetEffekseerManager()), (const EFK_CHAR*)filepath);
	m_handle = GraphicsEngine().GetEffectEngine().GetEffekseerManager().Play(m_effect, 0,0,0);
	GraphicsEngine().GetEffectEngine().GetEffekseerManager().SetSpeed(m_handle, speed);

	isPlayed = true;
}

void CEffect::Stop() {
	GraphicsEngine().GetEffectEngine().GetEffekseerManager().StopEffect(m_handle);
}