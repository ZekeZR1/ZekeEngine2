#pragma once
#include <stack>
class CVEditor;
class Searching;

class OnlineMatch : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
	bool IsSearchingRoom() { return m_isSearchingRoom; }
	void Disconnect();
private:
	SpriteRender* m_backSp = nullptr;
	SpriteRender* m_startSp = nullptr;
	SpriteRender* m_sp3 = nullptr;
	bool m_isSearchingRoom = false;
	CVEditor* m_editor = nullptr;
	std::string roomName;
	Searching* m_searching = nullptr;

	int m_cs = 0;
	unsigned int m_sumLag = 0;
	unsigned short m_sec = 0;
	double delta = 0.0;
	int m_mSec = 0;
};

class Searching : public GameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	SpriteRender* m_sp = nullptr;
	CVector3 m_pos = { -270,30,0 };
	CQuaternion m_rot = CQuaternion::Identity();
};