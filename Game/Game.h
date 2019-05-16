#pragma once
//#include "GameCamera.h"


class Game : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	SkinModelRender* m_model = nullptr;
};

