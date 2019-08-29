#include "stdafx.h"
#include "OnlineMatch.h"
#include "Network/NetworkLogic.h"

bool OnlineMatch::Start() {
	NetSystem().CreateNetworkSystem();
	return true;
}

void OnlineMatch::OnDestroy() {
	NetSystem().DestroyNetworkSystem();
}

void OnlineMatch::Update(){
	NetSystem().GetNetworkLogic().Update();
}