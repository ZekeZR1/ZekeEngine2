#include "stdafx.h"
#include "..//ZekeEngine2/Engine.h"
#include "OnlineMatch.h"
#include "Network/NetworkLogic.h"
#include "Network/LoadBalancingListener.h"
#include "Game.h"
#include "..//..//ZekeEngine2/HID/CVEditor.h"

bool OnlineMatch::Start() {
	NetworkLogic::GetInstance().Start();

	m_sp1 = NewGO<SpriteRender>(0);
	m_sp1->Init(L"Assets/sprite/BlueTeamScoreBack.dds", 50, 50, true);

	m_sp2 = NewGO<SpriteRender>(0);
	m_sp2->Init(L"Assets/sprite/OrangeTeamScoreBack.dds", 50, 50, true);

	m_sp3 = NewGO<SpriteRender>(0);
	m_sp3->Init(L"Assets/sprite/TimerBack.dds", 50, 50, true);

	m_sp1->SetPosition({ 100,0,0 });
	m_sp2->SetPosition({ -100,0,0 });
	m_sp3->SetPosition({ 0,100,0 });

	m_editor = NewGO<CVEditor>(0);
	FontRender::FontInfo info;
	info.pos = { -10,200 };
	m_editor->SetEditorInfo(info);
	return true;
}

void OnlineMatch::OnDestroy() {

	if(m_cs!=0)
		NetworkLogic::GetInstance().GetLBL()->SetLagAve(m_sumLag / m_cs);

	DeleteGO(m_sp1);
	DeleteGO(m_sp2);
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
	m_sp1->SetCollisionTarget(cp);
	m_sp2->SetCollisionTarget(cp);
	m_sp3->SetCollisionTarget(cp);

	if (m_sp1->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
		auto str = m_editor->GetString();
		JString s = str.c_str();

		NetworkLogic::GetInstance().GetLBC()->opJoinOrCreateRoom(s, RoomOptions().setMaxPlayers(2));
		printf("Create or Join A Room\n");
		m_isSelectedRoom = true;
	}

	if (m_sp2->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
		NetworkLogic::GetInstance().GetLBC()->opJoinOrCreateRoom("B", RoomOptions().setMaxPlayers(2));
		printf("Create or Join B Room\n");
		m_isSelectedRoom = true;
	}

	if (m_sp3->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
		NetworkLogic::GetInstance().GetLBC()->opLeaveRoom();
		m_isSelectedRoom = false;
	}

	auto room = NetworkLogic::GetInstance().GetLBC()->getCurrentlyJoinedRoom();
	auto name = room.getName();
	auto pc = room.getPlayerCount();
	
	if (Pad(0).IsTrigger(enButtonA)) {
		//NewGO<Game>(0);
		//DeleteGO(this);
	}

	if (pc == 2 and m_isSelectedRoom) {
		NewGO<Game>(0);
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