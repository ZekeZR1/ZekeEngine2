#include "stdafx.h"
#include "ScoreManager.h"

ScoreManager::ScoreManager() {
}

ScoreManager::~ScoreManager() {

}


bool ScoreManager::Start() {
	static int SpPrio = 0;
	static int FnPrio = 1;

	m_min = m_gameScaleMin;

	//Orange Team Score
	m_orangeSp = NewGO<SpriteRender>(SpPrio);
	m_orangeSp->Init(L"Assets/sprite/OrangeTeamScoreBack.dds", 100, 100);
	m_orangeSp->SetPosition(OrangeSpPos);
	m_orangetFnt = NewGO<FontRender>(FnPrio);
	m_orangetFnt->Init(L"0", CVector2(OrangeSpPos.x, OrangeSpPos.y) + FontPosFix);

	//Timer
	m_timerSp = NewGO<SpriteRender>(SpPrio);
	m_timerSp->Init(L"Assets/sprite/TimerBack.dds", 250, 100);
	m_timerSp->SetPosition(TimerSpPos);
	m_timerFnt = NewGO<FontRender>(FnPrio);
	m_timerFnt->Init(L"5:00", CVector2(TimerSpPos.x,TimerSpPos.y) + FontPosFix);

	//Blue Team Score
	m_blueSp = NewGO<SpriteRender>(SpPrio);
	m_blueSp->Init(L"Assets/sprite/BlueTeamScoreBack.dds", 100, 100);
	m_blueSp->SetPosition({ -OrangeSpPos.x,OrangeSpPos.y,OrangeSpPos.z });
	m_blueFnt = NewGO<FontRender>(FnPrio);
	m_blueFnt->Init(L"0", CVector2(-OrangeSpPos.x, OrangeSpPos.y) + FontPosFix);

	return true;
}

void ScoreManager::OnDestroy() {
	DeleteGO(m_orangeSp);
	DeleteGO(m_blueSp);
	DeleteGO(m_timerSp);
	DeleteGO(m_blueFnt);
	DeleteGO(m_timerFnt);
	DeleteGO(m_orangetFnt);
}

void ScoreManager::Update() {
	if (m_isGameOver)
		return;

	m_sec -= IGameTime().GetFrameDeltaTime();

	if (m_sec <= 0) {
		if (m_min > 0) {
			m_min--;
			m_sec = 15;
		}
	}

	if (m_min <= 0 and m_sec <= 0) {
		m_min = 0;
		m_sec = 0;
		m_isGameOver = true;
	}

	currentTime = std::to_wstring(m_min);
	currentTime += L":";
	if (m_sec < 10) {
		currentTime += L"0";
	}
	currentTime += std::to_wstring(static_cast<int>(m_sec));
	m_timerFnt->SetText(currentTime.c_str());
}

void ScoreManager::Goal(eTeam team) {
	if (team == enOrangeTeam) {
		m_orangeTeamScore++;
		wchar_t str[256];
		swprintf_s(str, L"%d",m_orangeTeamScore);
		m_orangetFnt->Init(str, CVector2(OrangeSpPos.x, OrangeSpPos.y) + FontPosFix);
	}
	else if (team == enBlueTeam) {
		m_blueTeamScore++;
		wchar_t str[256];
		swprintf_s(str, L"%d", m_blueTeamScore);
		m_blueFnt->Init(str, CVector2(-OrangeSpPos.x, OrangeSpPos.y) + FontPosFix);
	}
}