#pragma once
#include "../GameObjects/Car.h"
class GameCamera;
class Stage;
class CPlayer;

class TestScene : public GameObject
{
public :
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	CPlayer* g_pPlayer = nullptr;
};

