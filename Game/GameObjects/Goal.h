#pragma once
class Ball;
class ScoreManager;
class Goal : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;

	void SetScoredFlag(bool flag) {
		m_isScored = flag;
	}

	bool IsScored() {
		return m_isScored;
	}

	void SetHostFlag(bool f) {
		m_isHost = f;
	}
private:
	Ball* mp_ball = nullptr;
	bool m_isScored = false;
	bool m_isHost = false;
	ScoreManager * mp_scoreManager = nullptr;
	//btGhostObject * m_ghost = nullptr;
	//btBoxShape* m_boxShape = nullptr;
};

