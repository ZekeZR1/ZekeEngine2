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
};

