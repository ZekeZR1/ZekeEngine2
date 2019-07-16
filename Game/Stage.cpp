#include "stdafx.h"
#include "Stage.h"


bool Stage::Start() {
	m_floorModel = NewGO<SkinModelRender>(0);
	m_floorModel->Init(L"Assets/modelData/Box.cmo");
	m_floorModel->SetPosition(CVector3::Zero());
	m_floorModel->SetRotation(CQuaternion::Identity());

	m_physicsStaticObject.CreateMeshObject(m_floorModel->GetSkinModel(), CVector3::Zero(),CQuaternion::Identity());
	return true;
}


void Stage::OnDestroy() {
	DeleteGO(m_floorModel);
}

void Stage::Update() {
	if (Pad(0).IsTrigger(enButtonA)) {
		m_floorModel->SetPosition(m_floorPos);
	}
}