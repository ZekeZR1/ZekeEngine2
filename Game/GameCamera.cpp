#include "stdafx.h"
#include "GameCamera.h"

bool GameCamera::Start() {
	return true;
}

void GameCamera::OnDestroy() {

}

void GameCamera::Update() {
	static const float moveSpeed = 1;
	/*if (Pad(0).IsPress(enButtonUp)) {
		m_pos.y += moveSpeed;
	}
	if (Pad(0).IsPress(enButtonDown)) {
		m_pos.y += -moveSpeed;
	}
	if (Pad(0).IsPress(enButtonLeft)) {
		m_pos.z += moveSpeed;
	}
	if (Pad(0).IsPress(enButtonRight)) {
		m_pos.z += -moveSpeed;
	}*/
/*
	MainCamera().SetPosition(m_pos);
	MainCamera().SetTarget(m_target);
	MainCamera().Update();*/
}