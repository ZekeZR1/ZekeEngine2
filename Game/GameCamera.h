#pragma once

class Car;

enum GameCameraMode {
	enBallCamera,
	enPlayerCamera,
	enAutoCamera
};

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

	void SetLocalCar(Car* pos) {
		m_car = pos;
	}

	void SetBallPos(CVector3 pos) {
		m_ballPos = pos;
	}

private:
	GameCameraMode m_mode = enBallCamera;
	CVector3 m_pos = { 0.f,0.f,-10.f };
	const float m_distanceMoveParam = 50.f;
	const float m_raiseViewPoint = 2.f;
	const float m_maxDistance = 650.f;
	const float m_minDistance = 200.f;
	float cameraRotationSpeed = 3.f;
	CVector3 m_ballPos = CVector3::Zero();
	Car* m_car = nullptr;

	CVector3 m_target = { 0.0f, 1.0f, 0.0f };
	CVector3 m_toCameraPos = { 0.0f, 0.0f, 5.f };
};

