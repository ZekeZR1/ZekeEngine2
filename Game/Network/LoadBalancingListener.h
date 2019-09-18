#pragma once
#include "LoadBalancing-cpp/inc/Client.h"
#include "BaseView.h"

#define PLAYER_UPDATE_INTERVAL_MS 500

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;

struct LocalPlayer
{
	LocalPlayer();
	int x;
	int y;
	int z;
	unsigned long lastUpdateTime;
};

class LoadBalancingListener : public ExitGames::LoadBalancing::Listener
{
public:
	LoadBalancingListener(BaseView* pView);
	~LoadBalancingListener();
	void setLBC(ExitGames::LoadBalancing::Client* pLbc);
	void connect(const ExitGames::Common::JString& userName);
	void disconnect();
	void createRoom(void);
	void service();
	
	void RaiseMyCarTransform(CVector3 pos, CQuaternion rot);
	/// <summary>
	/// �I�����C���l����Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	int GetOnlinePlayerCount() {
		return mpLbc->getCountPlayersOnline();
	}

	JString GetState();

	int GetLocalPlayerNumber() {
		return mLocalPlayerNr;
	}
private:
	enum RaiseEventCode {
		enPosition,
		enRotation
	};

	//From Common::BaseListener

	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

	//From LoadBalancing::LoadBalancingListener

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive);
	virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

	// callbacks for operations on PhotonLoadBalancing server
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster);
	virtual void disconnectReturn(void);
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void gotQueuedReturn(void);
	virtual void joinLobbyReturn(void);
	virtual void leaveLobbyReturn(void);
	virtual void onRoomListUpdate(void);
	virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
	virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
	virtual void onRoomPropertiesChange(const ExitGames::Common::Hashtable& changes);

	void updateState(void);
	void afterRoomJoined(int localPlayerNr);
private:
	
	ExitGames::LoadBalancing::Client* mpLbc;
	BaseView* mpView;
	int mMap = 1;	//���[���쐬���Ɏg��Key
	int m_val = 10; //���M����l�Ȃǂ�K���ɒ�`
	int m_maxPlayer = 2;


	int mLocalPlayerNr; //Photon���玩���Ɋ���U��ꂽ�v���C���[�i���o�[
	LocalPlayer mLocalPlayer;
	
	bool misConect = false;		//�Ȃ����Ă�`�H
	bool m_enemyAbandoned = false;
	bool misHang = false;		//���������Ă��Ă�H
	bool m_isEnemyLoadedMyData = false;
	bool m_isJoining = false;
};

