#pragma once
#include "LoadBalancingListener.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "TestView.h"

class TestView;


class NetworkLogic : Noncopyable
{
public:
	static NetworkLogic& GetInstance() {
		static NetworkLogic instance;
		return instance;
	}
	~NetworkLogic();
	void Start();
	void Connect();
	void Disconnect();
	void Update();

	LoadBalancingListener* GetLBL()
	{
		return mpLbl;
	}

	Client* GetLBC() {
		return mpLbc;
	}
private:
	ExitGames::LoadBalancing::Client* mpLbc;
	LoadBalancingListener* mpLbl = nullptr;
	TestView m_testview;
};
