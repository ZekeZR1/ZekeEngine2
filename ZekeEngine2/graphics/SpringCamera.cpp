#include "stdafx.h"
#include "SpringCamera.h"
#include "CCameraCollisionSlover.h"
float dampingK = 35.0f;
float CalcSpringScalar(
	float positionNow,
	float positionTarget,
	float& moveSpeed)
{
	float deltaTime =  1.0f / 30.0f;

	float dampingRate = 0.2f;
	float distance;
	distance = positionTarget - positionNow;
	if (fabsf(distance) < FLT_EPSILON) {
		moveSpeed = 0.0f;
		return positionTarget;
	}
	float originalDir = distance;
	originalDir /= fabsf(distance);
	float springAccel;
	springAccel = distance;

	float t = dampingK / (2.0f * dampingRate);
	float springK = t * t;
	springAccel *= springK;
	float vt = moveSpeed;
	vt *= dampingK;
	springAccel -= vt;
	springAccel *= deltaTime;
	moveSpeed += springAccel;

	float newPos = positionNow;
	float addPos = moveSpeed;
	addPos *= deltaTime;
	newPos += addPos;
	vt = positionTarget - newPos;
	if (fabsf(vt) < FLT_EPSILON) {
		newPos = positionTarget;
		moveSpeed = 0.0f;
	}
	else {
		vt /= fabsf(vt);
		if (vt * originalDir < 0.0f) {
			newPos = positionTarget;
			moveSpeed = 0.0f;
		}
	}
	return newPos;
}
CVector3 CalcSpringVector(
	const CVector3& positionNow,
	const CVector3& positionTarget,
	CVector3& moveSpeed,
	float maxMoveSpeed,
	float dampingRate
)
{
	float deltaTime = min(1.0f / 30.0f, 1.0f / 30.0f);
	//現在の位置と目標の位置との差分を求める。
	CVector3 distance;
	distance.Subtract(positionTarget, positionNow);
	CVector3 originalDir = distance;
	originalDir.Normalize();
	CVector3 springAccel;
	springAccel = distance;

	float t = dampingK / (2.0f * dampingRate);
	float springK = t * t;
	springAccel.Scale(springK);
	//加速度を決定。
	CVector3 vt = moveSpeed;
	vt.Scale(dampingK);
	springAccel.Subtract(vt);

	springAccel.Scale(deltaTime);
	moveSpeed.Add(springAccel);
	if (moveSpeed.LengthSq() > maxMoveSpeed*maxMoveSpeed) {
		//最高速度より速くなってしまった。
		moveSpeed.Normalize();
		moveSpeed.Scale(maxMoveSpeed);
	}
	CVector3 newPos = positionNow;
	if (moveSpeed.Length() < 1.0f) {
		newPos = positionTarget;
		moveSpeed = CVector3::Zero();
	}
	else {
		CVector3 addPos = moveSpeed;
		addPos.Scale(deltaTime);
		newPos.Add(addPos);
		vt.Subtract(positionTarget, newPos);
		vt.Normalize();
		if (vt.Dot(originalDir) < 0.0f) {
			newPos = positionTarget;
			moveSpeed = CVector3::Zero();
		}
	}
	return newPos;
}

SpringCamera::SpringCamera()
{
}


SpringCamera::~SpringCamera()
{
}

void SpringCamera::Init(
	Camera* camera,
	float maxMoveSpeed,
	bool isEnableCollisionSolver,
	float sphereCollisionRadius
)
{
	m_camera = camera;
	SetTarget(m_camera->GetTarget());
	SetPosition(m_camera->GetPosition());
	m_isEnableCollisionSolver = isEnableCollisionSolver;
	m_cameraCollisionSolver.Init(sphereCollisionRadius);
	m_targetMoveSpeed = CVector3::Zero();
	m_positionMoveSpeed = CVector3::Zero();
	m_maxMoveSpeed = maxMoveSpeed;
	m_isRefresh = true;
}

void SpringCamera::UpdateSpringCamera()
{
	if (m_camera == nullptr) {
		return;
	}
	if (m_isRefresh) {
		m_camera->SetTarget(m_target);
		m_camera->SetPosition(m_position);
		m_isRefresh = false;
	}
	else {
		m_dampingRate = CalcSpringScalar(m_dampingRate, m_targetDampingRate, m_dampingRateVel);
		CVector3 target = CalcSpringVector(m_camera->GetTarget(), m_target, m_targetMoveSpeed, m_maxMoveSpeed, m_dampingRate);
		CVector3 position = CalcSpringVector(m_camera->GetPosition(), m_position, m_positionMoveSpeed, m_maxMoveSpeed, m_dampingRate);
		m_camera->SetTarget(target);
		m_camera->SetPosition(position);
	}
}

void SpringCamera::Update()
{
	UpdateSpringCamera();
	if (m_isEnableCollisionSolver) {
		CVector3 result;
		m_cameraCollisionSolver.Execute(
			result,
			m_camera->GetPosition(),
			m_camera->GetTarget()
		);
		m_camera->SetPosition(result);
	}
	UpdateCamera();
}