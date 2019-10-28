#include "stdafx.h"
#include "GameCamera.h"
#include "Car.h"

//#define _USE_DEBUG_CAMERA

bool GameCamera::Start() {
	return true;
}

void GameCamera::OnDestroy() {

}

void GameCamera::Update() {

#ifdef _USE_DEBUG_CAMERA
	if (Pad(0).IsPress(enButtonUp)) {
		m_pos.y += 5.f;
	}
	if (Pad(0).IsPress(enButtonDown)) {
		m_pos.y -= 5.f;
	}
	if (Pad(0).IsPress(enButtonRight)) {
		m_pos.x += 5.f;
	}
	if (Pad(0).IsPress(enButtonLeft)) {
		m_pos.x -= 5.f;
	}
#else

	switch (m_mode) 
	{
	case enBallCamera:
	{
		CVector3 cameraPos = CVector3::Zero();
		auto carToBallVec = m_ballPos - m_car->GetPosition();
		carToBallVec.Normalize();
		cameraPos = m_car->GetPosition() + (carToBallVec * -8);
		if (cameraPos.y < 3.f)
			cameraPos.y = 3.f;
		else
			cameraPos.y = m_car->GetPosition().y + 3.f;

		m_pos = cameraPos;
		m_target = m_ballPos;
		m_target.y += m_raiseViewPoint;
	}
	break;
	case enPlayerCamera:
	{
		m_pos = (m_car->GetCarForward() * -10) + m_car->GetPosition();
		m_pos.y += 3.f;
		m_target = (m_car->GetCarForward() * 10) + m_car->GetPosition();
	}
	break;
	}


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
#endif
	MainCamera2D().Update();
	MainCamera().SetTarget(m_target);
	MainCamera().SetPosition(m_pos);
	MainCamera().Update();
}