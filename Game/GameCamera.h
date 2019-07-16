#pragma once
class GameCamera: public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;

	void SetTarget(CVector3 pos) {
		m_target = pos;
	}

	void SetCameraPosition(CVector3 pos) {
		m_pos = pos;
	}

	CVector3& GetCameraPos() {
		return m_pos;
	}

private:
	CVector3 m_pos = { 0,5,-10 };
	CVector3 m_target = CVector3::Zero();
};

