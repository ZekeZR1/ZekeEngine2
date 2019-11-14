#include "stdafx.h"
#include "..//ZekeEngine2/Engine.h"
#include "OnlineMatch.h"
#include "Network/NetworkLogic.h"
#include "Network/LoadBalancingListener.h"
#include "Game.h"
#include "..//..//ZekeEngine2/HID/CVEditor.h"
#include "Title.h"

bool OnlineMatch::Start() {
	NetworkLogic::GetInstance().Start();

	m_backSp = NewGO<SpriteRender>(3);
	m_backSp->Init(L"Assets/sprite/online.dds",900,500);

	m_startSp = NewGO<SpriteRender>(3);
	m_startSp->Init(L"Assets/sprite/start.dds", 300, 80, true);

	m_sp3 = NewGO<SpriteRender>(3);
	m_sp3->Init(L"Assets/sprite/TimerBack.dds", 50, 50, true);

	m_startSp->SetPosition({ 250,-130,0 });
	m_sp3->SetPosition({ 0,100,-130 });

	m_editor = NewGO<CVEditor>(0);
	m_editor->CreateText(5);
	FontRender::FontInfo info;
	info.pos = { -100,70 };
	m_editor->SetEditorInfo(info);

	return true;
}

void OnlineMatch::OnDestroy() {

	if(m_cs!=0)
		NetworkLogic::GetInstance().GetLBL()->SetLagAve(m_sumLag / m_cs);

	DeleteGO(m_startSp);
	DeleteGO(m_backSp);
	DeleteGO(m_sp3);
	DeleteGO(m_editor);
}

void OnlineMatch::Update() {

	SYSTEMTIME st;
	GetLocalTime(&st);
	auto lstate = ZKeyBoard().GetStateTracker().GetLastState();
	//if(ZKeyBoard().IsTrigger(DirectX::Keyboard::State::Space)) {
	//}
	//auto lstate = ZKeyBoard().GetStateTracker().GetLastState();
	//auto lag = NetworkLogic::GetInstance().GetLBL()->GetLag();
	//if (lag < 999) {
	//	m_cs++;
	//	float lg = (float)lag / 1000.f;
	//	m_sumLag += lg;
	//	printf("sum lag %d\n", m_sumLag);
	//}

	//printf("%02d.%03d\n",
	//	st.wSecond,
	//	st.wMilliseconds);

	//if (m_sec != st.wSecond) {
	//	delta = 100 - st.wMilliseconds;
	//}
	//else {
	//	delta = st.wMilliseconds - m_mSec;
	//}

	//printf("Delta %f\n", delta);

	//m_sec = st.wSecond;
	//m_mSec = st.wMilliseconds;


	NetworkLogic::GetInstance().Update();

	auto state = NetworkLogic::GetInstance().GetLBC()->getState();
	if (state == 6) {
		NetworkLogic::GetInstance().GetLBC()->opJoinLobby();
	}

	auto cp = Mouse::GetCursorPos();
	m_startSp->SetCollisionTarget(cp);
	m_sp3->SetCollisionTarget(cp);

	if (Pad(0).IsTrigger(enButtonStart) or (m_startSp->isCollidingTarget() and Mouse::IsTrigger(enLeftClick))) {
		auto str = m_editor->GetString();
		JString roomName = str.c_str();

		NetworkLogic::GetInstance().GetLBC()->opJoinOrCreateRoom(roomName, RoomOptions().setMaxPlayers(2));
		printf("Create or Join Room\n");
		
		m_startSp->SetMulCol({ 0.5,0.5,0.5 ,1 });
		m_editor->SetActiveFlag(false);
		m_searching = NewGO<Searching>(6);

		m_isSearchingRoom = true;
	}

	if (ZKeyBoard().GetStateTracker().pressed.Escape or m_sp3->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
		if (m_isSearchingRoom) {
			NetworkLogic::GetInstance().GetLBC()->opLeaveRoom();
			m_startSp->SetMulCol({ 1,1,1 ,1 });
			m_editor->SetActiveFlag(true);
			DeleteGO(m_searching);
			m_isSearchingRoom = false;
		}
	}

	auto room = NetworkLogic::GetInstance().GetLBC()->getCurrentlyJoinedRoom();
	auto name = room.getName();
	auto pc = room.getPlayerCount();

	if (pc == 2 and m_isSearchingRoom) {
		NewGO<Game>(0);
		DeleteGO(FindGO<Title>("TitleScene"));
		DeleteGO(this);
	}


	if (Pad(0).IsPress(enButtonStart)) {
		int cgr = NetworkLogic::GetInstance().GetLBC()->getCountGamesRunning();
		int cpig = NetworkLogic::GetInstance().GetLBC()->getCountPlayersIngame();
		int cpo = NetworkLogic::GetInstance().GetLBC()->getCountPlayersOnline();

		NetworkLogic::GetInstance().GetLBL()->RaiseCurrentLocalTime();

		auto state = NetworkLogic::GetInstance().GetLBL()->GetState();

		wprintf(state);
		puts("");

		if (NetworkLogic::GetInstance().GetLBC()->getIsInRoom()) {
			printf("is in room now\n");
		}
		else {
			puts("isnt in room now");
		}

		auto lpn = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerNumber();
		wprintf(L"Local Player Number : %d\n", lpn);

		printf("Room Player Count : %d\n", pc);
		wprintf(name);
		printf("\n");
		//auto name = NetSystem().GetNetworkLogic().GetLBC()->getCurrentlyJoinedRoom().getName();

		//printf("Count Games Running : %d\n", cgr);
		//printf("Count Players In Game : %d\n", cpig);
		//printf("Count Players Online : %d\n", cpo);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool Searching::Start() {
	m_sp = NewGO<SpriteRender>(6);
	m_sp->Init(L"Assets/sprite/searching.dds", 80, 80);
	m_sp->SetPosition(m_pos);
	return true;
}

void Searching::OnDestroy() {
	DeleteGO(m_sp);
}

void Searching::Update() {
	CQuaternion rot = CQuaternion::Identity();
	rot.SetRotationDeg({ 0,0,1 }, 4);
	m_rot.Multiply(rot);
	m_sp->SetRotation(m_rot);
}
