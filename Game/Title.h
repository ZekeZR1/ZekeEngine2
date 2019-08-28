#pragma once
class Title : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update();
private:
	SkinModelRender* m_car;
	SkinModelRender* m_ground;
};

