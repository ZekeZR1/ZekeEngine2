#pragma once

//BaseViewを継承して、ここでオーバライドしたものをLoadBalancingListenerから呼ぶようにしてる
class TestView : public BaseView
{
public:
	TestView();
	~TestView();
	void SetLBC(ExitGames::LoadBalancing::Client* lbc, LoadBalancingListener* lbl);
	virtual void updateState(int state, const ExitGames::Common::JString& stateStr, const ExitGames::Common::JString& joinedRoomName);
private:
	ExitGames::LoadBalancing::Client* mpLbc;
	LoadBalancingListener* mpLbl = nullptr;
};


