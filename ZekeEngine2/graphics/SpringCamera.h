#pragma once
#include "Camera.h"
#include "CCameraCollisionSlover.h"
class CCameraCollisionSlover;

class SpringCamera
{
public:
	SpringCamera();
	~SpringCamera();
	void Init(
		Camera* camera,
		float maxMoveSpeed,
		bool isEnableCollisionSolver,
		float sphereCollisionRadius
	);

	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}

	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}

	void SetFar(float _far)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetFar(_far);
	}

	void SetNear(float _near)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetNear(_near);
	}

	const CMatrix& GetCameraRotation() const
	{
		if (m_camera == nullptr) {
			return CMatrix::Identity();
		}
		return m_camera->GetCameraRotation();
	}

	const CVector3& GetTarget() const
	{
		return m_target;
	}

	const CVector3& GetPosition() const
	{
		return m_position;
	}

	const CVector3& GetRealTarget() const
	{
		if (m_camera == nullptr) {
			return CVector3::Zero();
		}
		return m_camera->GetTarget();

	}

	const CVector3& GetRealPosition() const
	{
		if (m_camera == nullptr) {
			return CVector3::Zero();
		}
		return m_camera->GetPosition();
	}

	void Update();

	const Camera* GetCamera() const
	{
		return m_camera;
	}

	void SetDampingRate(float dampingRate)
	{
		m_targetDampingRate = dampingRate;
	}

	Camera* GetCamera()
	{
		return m_camera;
	}
	void SetViewAngle(float angle)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetViewAngle(angle);
	}
	float GetViewAngle() const
	{
		if (m_camera == nullptr) {
			return 0.0f;
		}
		return m_camera->GetViewAngle();
	}

	void Refresh()
	{
		m_targetMoveSpeed = CVector3::Zero();
		m_positionMoveSpeed = CVector3::Zero();
		m_isRefresh = true;
	}

	void UpdateSpringCamera();

	void UpdateCamera()
	{
		if (m_camera) {
			m_camera->Update();
		}
	}
private:
	Camera * m_camera = nullptr;
	CVector3	m_target = CVector3::Zero();
	CVector3	m_position = CVector3::Zero();
	CVector3	m_targetMoveSpeed = CVector3::Zero();
	CVector3	m_positionMoveSpeed = CVector3::Zero();
	float		m_maxMoveSpeed = 0.0f;
	float		m_targetDampingRate = 1.0f;
	float		m_dampingRate = 1.0f;
	float		m_dampingRateVel = 0.0f;
	bool		m_isEnableCollisionSolver = false;
	bool		m_isRefresh = true;
	CCameraCollisionSlover m_cameraCollisionSolver;
};

