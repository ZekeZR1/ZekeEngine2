#pragma once
class OnlineMatch : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	SpriteRender* m_sp1 = nullptr;
	SpriteRender* m_sp2 = nullptr;
	SpriteRender* m_sp3 = nullptr;
	bool m_isSelectedRoom = false;

	int m_cs = 0;
	unsigned int m_sumLag = 0;
	unsigned short m_sec = 0;
	double delta = 0.0;
	int m_mSec = 0;
};

