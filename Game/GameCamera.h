#pragma once
class GameCamera: public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;

	void SetTarget(CVector3 pos) {
		m_target = pos;
	}

	void SetCameraPosition(CVector3 pos) {
		m_pos = pos;
	}

	CVector3& GetCameraPos() {
		return m_pos;
	}

private:
	CVector3 m_pos = { 0.f,0.f,-10.f };
	const float m_distanceMoveParam = 50.f;
	const float m_raiseViewPoint = 2.f;
	const float m_maxDistance = 650.f;
	const float m_minDistance = 200.f;
	float cameraRotationSpeed = 1.f;
	CVector3 m_target = { 0.0f, 1.0f, 0.0f };
	CVector3 m_toCameraPos = { 0.0f, 0.0f, 5.f };
};

