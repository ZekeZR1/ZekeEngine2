#pragma once
class ScoreManager : public GameObject
{
public:

	//TODO : ���_���̒ǉ��Q�[���p�̃}�l�[�W���[�@�\����������B

	ScoreManager();
	~ScoreManager();

	bool Start() override;
	void OnDestroy() override;
	void Update() override;

	enum eTeam {
		enOrangeTeam,
		enBlueTeam
	};

	/// <summary>
	/// �S�[�������܂������ɌĂԊ֐�
	/// </summary>
	/// <param name="team">���F�̃`�[�������_��������</param>
	void Goal(eTeam team);

	/// <summary>
	/// ���Ԃɂ���ăQ�[�����I��������ǂ����̃t���O��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	bool IsGameOver() const{
		return m_isGameOver;
	}
private:
	int m_blueTeamScore = 0;
	int m_orangeTeamScore = 0;

	SpriteRender* m_orangeSp = nullptr;
	SpriteRender* m_blueSp = nullptr;
	SpriteRender* m_timerSp = nullptr;

	FontRender* m_orangetFnt = nullptr;
	FontRender* m_blueFnt = nullptr;
	FontRender* m_timerFnt = nullptr;

	std::wstring currentTime;

	//1���������̃Q�[����
	int m_gameScaleMin = 1;

	bool m_isGameOver = false;
	int m_min = 5;
	float m_sec = 0;
	//pos
	CVector3 OrangeSpPos = { -160,320,0 };
	CVector3 TimerSpPos = { 0,320,0 };
	CVector2 FontPosFix = { -30,40 };
};

