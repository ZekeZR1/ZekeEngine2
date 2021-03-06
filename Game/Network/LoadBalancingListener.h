#pragma once
#include "LoadBalancing-cpp/inc/Client.h"
#include "BaseView.h"
#include "../GameObjects/Car.h"

#define PLAYER_UPDATE_INTERVAL_MS 500

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;

struct PlayerView {
	CVector3 pos = CVector3::Zero();
	CQuaternion rot = CQuaternion::Identity();
	CVector3 linearVelocity = CVector3::Zero();
	CVector3 angularVelocity = CVector3::Zero();
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

	void RaiseCurrentLocalTime();

	void RaiseForceAndTorque(CVector3 force, CVector3 torque,int carNumber);

	void RaiseCarTransform(CVector3 pos, CQuaternion rot, int carNumber);

	void RaiseBallTransform(CVector3 pos, CQuaternion rot);

	void RaiseGameScore(int blue, int orange);

	/// <summary>
	/// ローカルプレイヤーのコントローラーのインプット状況を送信する。
	/// </summary>
	/// <param name="input">ローカルプレイヤーの入力情報構造体</param>
	void RaiseLocalPlayerInput(Car::CarControll input);

	/// <summary>
	/// オンライン人数を返します
	/// </summary>
	/// <returns></returns>
	int GetOnlinePlayerCount() {
		return mpLbc->getCountPlayersOnline();
	}

	/// <summary>
	/// 自分がゲームホストか
	/// </summary>
	/// <returns>ホストの場合trueを返します</returns>
	bool IsHost() {
		return GetLocalPlayerNumber() < GetEnemyPlayerNumber();
	}

	JString GetState();

	int GetLocalPlayerNumber() {
		return mLocalPlayerNr;
	}

	int GetEnemyPlayerNumber() {
		return m_enemyPlayerNumber;
	}

	/// <summary>
/// 敵のコントローラー入力状態を取得
/// </summary>
/// <returns>対戦相手の入力値構造体</returns>
	Car::CarControll GetEnemeyCarInputs() {
		return m_enemyCon;
	}
	enum RaiseEventCode {
		enEnemyTransform,
		enMyTransform,
		enTime,
		enInputs,
		enPing,
		enForceAndTorque,
		enEnemyCarVelocity,
		enMyCarVelocity,
		enBallTransform,
		enScore,
	};

	void SetCars(Car* localCar, Car* enemyCar) {
		m_localPlayerCar = localCar;
		m_onlinePlayerCar = enemyCar;
	}

	short GetLag() {
		return m_lag;
	}

	short GetLagAve() {
		return m_lagAve;
	}

	void SetLagAve(int ave) {
		m_lagAve = ave;
	}

	PlayerView GetOnlinePlayerView(){
		return m_onlinePlayer;
	}

	PlayerView GetLocalPlayerView() {
		return m_localPlayer;
	}

	CVector3 GetBallPos() {
		return m_ballPos;
	}

	CQuaternion GetBallRot() {
		return m_ballRot;
	}

	int GetBlueTeamScore() {
		return m_blueTeamScore;
	}

	int GetOrangeTeamScore() {
		return m_orangeTeamScore;
	}
private:

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
	virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegions*/, const ExitGames::Common::JVector<ExitGames::Common::JString>&) override;
	void updateState(void);
	void afterRoomJoined(int localPlayerNr);

private:
	ExitGames::LoadBalancing::Client* mpLbc;
	BaseView* mpView;
	int mMap = 1;	//ルーム作成時に使うKey
	int m_val = 10; //送信する値などを適当に定義
	int m_maxPlayer = 2;

	int mLocalPlayerNr; //Photonから自分に割り振られたプレイヤーナンバー
	int m_enemyPlayerNumber = 0;
	//LocalPlayer mLocalPlayer;
	unsigned long lastUpdateTime;
	bool misConect = false;		
	bool m_enemyAbandoned = false;
	bool misHang = false;		
	bool m_isEnemyLoadedMyData = false;
	bool m_isJoining = false;

	Car* m_localPlayerCar = nullptr;
	Car* m_onlinePlayerCar = nullptr;

	int m_lag = 999;
	int m_lagAve = 999;

	PlayerView m_localPlayer;
	PlayerView m_onlinePlayer;

	//CVector3 m_EnemyLinearVelocity = CVector3::Zero();
	//CVector3 m_EnemyAngularVelocity = CVector3::Zero();

	//CVector3 m_MyLinearVelocity = CVector3::Zero();
	//CVector3 m_MyAngularVelocity = CVector3::Zero();

	CVector3 m_ballPos = CVector3::Zero();
	CQuaternion m_ballRot = CQuaternion::Identity();

	//CVector3 m_enemyPos = CVector3::Zero();
	//CVector3 m_myPos = CVector3::Zero();

	int m_blueTeamScore = 0;
	int m_orangeTeamScore = 0;
	//Key
	const nByte key = 103;

	enum DataKey {
		enAccel,
		enBack,
		enSteering,
		enAerealX,
		enAerealY,
		enJump,
		enBoost,
		enAirRoll
	};

	const nByte ACC = enAccel;
	const nByte BACK = enBack;
	const nByte STEER = enSteering;
	const nByte AX = enAerealX;
	const nByte AY = enAerealY;
	const nByte JUMP = enAerealY;
	const nByte BOOST = enAerealY;
	const nByte AIRR = enAerealY;

	Car::CarControll m_enemyCon;
};

