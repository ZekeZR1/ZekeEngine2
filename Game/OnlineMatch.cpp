#include "stdafx.h"
#include "..//ZekeEngine2/Engine.h"
#include "OnlineMatch.h"
#include "Network/NetworkLogic.h"
#include "Network/LoadBalancingListener.h"
#include "Game.h"

bool OnlineMatch::Start() {
	NetSystem().CreateNetworkSystem();
	m_sp1 = NewGO<SpriteRender>(0);
	m_sp1->Init(L"Assets/sprite/BlueTeamScoreBack.dds", 50, 50, true);

	m_sp2 = NewGO<SpriteRender>(0);
	m_sp2->Init(L"Assets/sprite/OrangeTeamScoreBack.dds", 50, 50, true);

	m_sp3 = NewGO<SpriteRender>(0);
	m_sp3->Init(L"Assets/sprite/TimerBack.dds", 50, 50, true);

	m_sp1->SetPosition({ 100,0,0 });
	m_sp2->SetPosition({ -100,0,0 });
	m_sp3->SetPosition({ 0,100,0 });

	auto lbc = NetSystem().GetNetworkLogic().GetLBC();

	lbc->opLeaveRoom();

	return true;
}

void OnlineMatch::OnDestroy() {
	//NetSystem().DestroyNetworkSystem();
}

void OnlineMatch::Update(){
	NetSystem().GetNetworkLogic().Update();
	auto lbc = NetSystem().GetNetworkLogic().GetLBC();
	auto lbl = NetSystem().GetNetworkLogic().GetLBL();

	auto cp = Mouse::GetCursorPos();
	m_sp1->SetCollisionTarget(cp);
	m_sp2->SetCollisionTarget(cp);
	m_sp3->SetCollisionTarget(cp);



	if (m_sp1->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
		//NetSystem().GetNetworkLogic().Connect();
		NetSystem().GetNetworkLogic().GetLBC()->opJoinOrCreateRoom("A", RoomOptions().setMaxPlayers(2));
		printf("Create or Join A Room\n");
	}

	if (m_sp2->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
			//NetSystem().GetNetworkLogic().Connect();
			//NetSystem().GetNetworkLogic().GetLBC()->opJoinOrCreateRoom("B", RoomOptions().setMaxPlayers(2));
		NetSystem().GetNetworkLogic().GetLBC()->opJoinOrCreateRoom("B", RoomOptions().setMaxPlayers(2));
		printf("Create or Join B Room\n");
	}

	if (m_sp3->isCollidingTarget() and Mouse::IsTrigger(enLeftClick)) {
		NetSystem().GetNetworkLogic().GetLBC()->opLeaveRoom();
	}

	auto room = NetSystem().GetNetworkLogic().GetLBC()->getCurrentlyJoinedRoom();
	auto name = room.getName();
	auto pc = room.getPlayerCount();

	if (pc == 2) {
		NewGO<Game>(0);
		DeleteGO(this);
	}


	if (Pad(0).IsPress(enButtonStart)) {
		int cgr = NetSystem().GetNetworkLogic().GetLBC()->getCountGamesRunning();
		int cpig = NetSystem().GetNetworkLogic().GetLBC()->getCountPlayersIngame();
		int cpo = NetSystem().GetNetworkLogic().GetLBC()->getCountPlayersOnline();


		auto state = lbl->GetState();

		wprintf(state);
		puts("");

		if (lbc->getIsInRoom()) {
			printf("is in room now\n");
		}
		else {
			puts("isnt in room now");
		}

		auto lpn = lbl->GetLocalPlayerNumber();
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