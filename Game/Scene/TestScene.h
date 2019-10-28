#pragma once
#include "../GameObjects/Car.h"
class GameCamera;
class Stage;

class TestScene : public GameObject
{
public :
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
	Car::CarControll m_carCon;
	Car* m_myCar = nullptr;
	Car* m_enemyCar = nullptr;
};

