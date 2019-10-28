#pragma once
#include "..//GameObjects/Car.h"

class Stage;
class Ball;
class GameCamera;
class ScoreManager;

class Game : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
	void PostUpdate() override;
private:
	void SetInputs();
	void RaiseInputs();
	void RaiseCarVelocitys();
	void RaiseBallTransform();
	void RaiseBallVelocity();
	void RaiseCarTransform();
	void RaiseGameTime();
	void RaiseGameScore();
	Car::CarControll m_carCon;
	Car* m_myCar = nullptr;
	Car* m_enemyCar = nullptr;
	Ball* m_ball = nullptr;
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
	ScoreManager* m_scoreManager;
	int m_raiseTimer = 0;

	float m_NetworkLagTime = 0;
	float m_lagFixTimer = 0;
	unsigned short m_lag = 0;
	std::queue<Car::CarControll> m_inputDataQueue;

	CVector3 m_localCarInitPos = {0,2,-50};
	CVector3 m_onlineCarInitPos = {0,2,50};
};

