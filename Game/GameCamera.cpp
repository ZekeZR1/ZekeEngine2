#include "stdafx.h"
#include "GameCamera.h"

bool GameCamera::Start() {
	return true;
}

void GameCamera::OnDestroy() {

}

void GameCamera::Update() {
	MainCamera2D().Update();

	m_target.y += m_raiseViewPoint;

	float x = Pad(0).GetRStickXF();
	float y = Pad(0).GetRStickYF();

	//Y軸周りの回転
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), cameraRotationSpeed * x);
	qRot.Multiply(m_toCameraPos);

	CVector3 toCameraPosOld = m_toCameraPos;

	//X軸周りの回転。
	CVector3 axisX;
	axisX.Cross(CVector3::AxisY(), m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, cameraRotationSpeed * y);
	qRot.Multiply(m_toCameraPos);
	CVector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	if (toPosDir.y < -0.5f) {
		//カメラ上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//カメラ下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	CVector3 pos = m_target + m_toCameraPos;

	MainCamera().SetTarget(m_target);
	MainCamera().SetPosition(m_pos);
	MainCamera().Update();
}