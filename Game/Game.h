#pragma once
#include "Car.h"

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
private:
	void SetInputs();
	void RaiseInputs();
	Car::CarControll m_carCon;
	Car* m_myCar = nullptr;
	Car* m_enemyCar = nullptr;
	Ball* m_ball = nullptr;
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
	ScoreManager* m_scoreManager;
	int m_raiseTimer = 0;
};

