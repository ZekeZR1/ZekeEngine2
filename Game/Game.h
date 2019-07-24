#pragma once
//#include "GameCamera.h"
#include "Vehicle.h"
#include "Car.h"
class Stage;
class Vehicle;
class GameCamera;

class Game : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	Car m_car;
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
};

