#pragma once
class Ball;
class Goal : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	Ball* mp_ball = nullptr;
	//btGhostObject * m_ghost = nullptr;
	//btBoxShape* m_boxShape = nullptr;
};

