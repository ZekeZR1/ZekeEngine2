#pragma once
#include "..//GameObjects/Car.h"

class Stage;
class Ball;
class GameCamera;
class ScoreManager;

class Training : public GameObject
{
public :
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	Car::CarControll m_carCon;
	Car* m_myCar = nullptr;
	CVector3 m_CarInitPos = { 0,2,-50 };

	Ball* m_ball = nullptr;
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
	ScoreManager* m_scoreManager;
};

