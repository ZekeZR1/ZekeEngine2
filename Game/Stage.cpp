#include "stdafx.h"
#include "Stage.h"
#include "Goal.h"

bool Stage::Start() {
	m_floorModel = NewGO<SkinModelRender>(0);
	m_floorModel->Init(L"Assets/modelData/RLStage.cmo");
	m_floorModel->SetShadowCasterFlag(false);
	m_floorModel->SetPosition(CVector3::Zero());
	m_floorModel->SetRotation(CQuaternion::Identity());
	m_goal = NewGO<Goal>(0);
	m_physicsStaticObject.CreateMeshObject(m_floorModel->GetSkinModel(), CVector3::Zero(),CQuaternion::Identity(),0.5f);

	return true;
}


void Stage::OnDestroy() {
	DeleteGO(m_goal);
	DeleteGO(m_floorModel);
}

void Stage::Update() {
}