#pragma once
#include "LoadBalancingListener.h"
//#include "../../Game/Online/LoadBalancingListener.h"
#include "LoadBalancing-cpp/inc/Client.h"
//#include "../../Game/Online/TestView.h"
#include "TestView.h"
class TestView;
class NetworkLogic
{
public:
	NetworkLogic();
	~NetworkLogic();
	void Start();
	void Disconnect();
	void Update();

	LoadBalancingListener* GetLBL()
	{
		return mpLbl;
	}
private:
	ExitGames::LoadBalancing::Client* mpLbc;
	LoadBalancingListener* mpLbl = nullptr;
	TestView m_testview;
};


class INetworkSystem : Noncopyable {
public:
	NetworkLogic& GetNetworkLogic(){
		return m_network;
	}
	void CreateNetworkSystem() {
		m_network.Start();
	}
	void DestroyNetworkSystem() {
		m_network.Disconnect();
	}
private:
	NetworkLogic m_network;
};

static INetworkSystem& NetSystem() {
	static INetworkSystem* instance = nullptr;
	if (instance == nullptr) {
		instance = new INetworkSystem;
	}
	return *instance;
}