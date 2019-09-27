#pragma once
class ScoreManager : public GameObject
{
public:

	//TODO : 同点時の追加ゲーム用のマネージャー機能を実装する。

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
	/// ゴールが決まった時に呼ぶ関数
	/// </summary>
	/// <param name="team">何色のチームが得点をしたか</param>
	void Goal(eTeam team);

	/// <summary>
	/// 時間によってゲームが終わったかどうかのフラグを返します。
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

	//1試合何分のゲームか
	int m_gameScaleMin = 1;

	bool m_isGameOver = false;
	int m_min = 5;
	float m_sec = 0;
	//pos
	CVector3 OrangeSpPos = { -160,320,0 };
	CVector3 TimerSpPos = { 0,320,0 };
	CVector2 FontPosFix = { -30,40 };
};

