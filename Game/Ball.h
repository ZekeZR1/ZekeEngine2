#pragma once
#include "../ZekeEngine2/physics/CSphereCollider.h"
#include "../ZekeEngine2/physics/RigitBody.h"

class Ball : public GameObject
{
public:
	void Awake() override;
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
	void ResetBall();

	CVector3 GetPosition() {
		return m_ballModel->GetPosition();
	}
	CQuaternion GetRotation() {
		return m_ballModel->GetRotation();
	}

	void SetTransform(CVector3 pos,CQuaternion rot);
private:
	//graphics
	SkinModelRender* m_ballModel = nullptr;

	//physics
	CSphereCollider m_col;
	RigidBody m_rb;
	float m_radius = 3.f;
	float m_mass = 1.1f;
	float m_damping = 3.f;
	float m_restitution = 1.5f;
};

