#pragma once
class Car;
class OnlineMatch;
class Stage;
class TitleMenu;

class Title : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update();
private:
	Car* m_myCar = nullptr;
	Stage* m_stage = nullptr;
	TitleMenu* m_menu = nullptr;
	OnlineMatch* m_onlineMatch = nullptr;
	SkinModelRender* m_ground;
	SpriteRender* m_spriteRender = nullptr;
};

