#pragma once
class TitleMenu : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	enum MenuButton {
		enOnlineMatch,
		enTraining,
		enOption,
		enNumMenu
	};

	int m_selecting = enOnlineMatch;

	std::vector<SpriteRender*> m_menus;
	std::vector<FontRender*> m_fonts;
	CVector3 m_menuTopPos = { -450,-150,0 };
	float m_menuDist = 80.f;
};

