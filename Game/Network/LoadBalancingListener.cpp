#include "stdafx.h"
#include "BaseView.h"
#include "OutputListener.h"
#include "..//Car.h"
#include "LoadBalancingListener.h"
#include "NetworkLogic.h"
#include "Console.h"
#include "TestView.h"
#include <fstream>
#include <string>

const JString PeerStatesStr[] = {
	L"Uninitialized",
	L"PeerCreated",
	L"ConnectingToNameserver",
	L"ConnectedToNameserver",
	L"DisconnectingFromNameserver",
	L"Connecting",
	L"Connected",
	L"WaitingForCustomAuthenticationNextStepCall",
	L"Authenticated",
	L"JoinedLobby",
	L"DisconnectingFromMasterserver",
	L"ConnectingToGameserver",
	L"ConnectedToGameserver",
	L"AuthenticatedOnGameServer",
	L"Joining",
	L"Joined",
	L"Leaving",
	L"Left",
	L"DisconnectingFromGameserver",
	L"ConnectingToMasterserver",
	L"ConnectedComingFromGameserver",
	L"AuthenticatedComingFromGameserver",
	L"Disconnecting",
	L"Disconnected"
};

// TODO: update PeerStatesStr when PeerStates changes
// Checker below checks count match only
class PeerStatesStrChecker
{
public:
	PeerStatesStrChecker(void)
	{
		DEBUG_ASSERT(sizeof(PeerStatesStr) / sizeof(JString) == PeerStates::Disconnected + 1);
	}
} checker;

//LocalPlayer::LocalPlayer(void) : x(0), y(0),z(0), lastUpdateTime(0)
//{
//}

LoadBalancingListener::LoadBalancingListener(BaseView* pView)
{
	mpLbc = NULL;
	mpView = pView;
}

LoadBalancingListener::~LoadBalancingListener(void)
{
	//delete mpView;
}

void LoadBalancingListener::setLBC(ExitGames::LoadBalancing::Client* pLbc)
{
	this->mpLbc = pLbc;
}

void LoadBalancingListener::connect(const JString& userName)
{
	mpLbc->connect(AuthenticationValues().setUserID(JString() + GETTIMEMS()), userName);
}

void LoadBalancingListener::disconnect() {
	mpLbc->disconnect();
	misConect = false;
}


void LoadBalancingListener::debugReturn(int debugLevel, const JString& string)
{
	Console::get().debugReturn(debugLevel, string);
}

void LoadBalancingListener::connectionErrorReturn(int errorCode)
{
	Console::get().writeLine(JString(L"connection failed with error ") + errorCode);
	updateState();
}

void LoadBalancingListener::clientErrorReturn(int errorCode)
{
	Console::get().writeLine(JString(L"received error ") + errorCode + L" from client");
	updateState();
}

void LoadBalancingListener::warningReturn(int warningCode)
{
	Console::get().writeLine(JString(L"received warning ") + warningCode + " from client");
}

void LoadBalancingListener::serverErrorReturn(int errorCode)
{
	Console::get().writeLine(JString(L"received error ") + errorCode + " from server");
	updateState();
}

void LoadBalancingListener::joinRoomEventAction(int playerNr, const JVector<int>& playernrs, const Player& player)
{
	if (mLocalPlayerNr != playerNr)
	{
		//つながった！
		m_enemyPlayerNumber = playerNr;
		misConect = true;
		m_enemyAbandoned = false;
	}
	else {
		m_isJoining = true;
	}

	Console::get().writeLine(JString("player ") + playerNr + L" " + player.getName() + L" has joined the game");
}

void LoadBalancingListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	if (isInactive)
		Console::get().writeLine(JString(L"player ") + playerNr + L" has suspended the game");
	else
	{
		Console::get().writeLine(JString(L"player ") + playerNr + L" has abandoned the game");
	}
	if (mLocalPlayerNr == playerNr) {
		m_isJoining = false;
	}
	else {
		m_enemyAbandoned = true;
	}
	misConect = false;//切れた。
}

void LoadBalancingListener::RaiseCarTransform(CVector3 pos, CQuaternion rot, int carNumber) {
	Hashtable data;
	float coords[] = { static_cast<float>(pos.x),static_cast<float>(pos.y),static_cast<float>(pos.z) };
	data.put((nByte)1, coords, 3);
	switch (carNumber) {
	case 0:
		mpLbc->opRaiseEvent(false, data, enMyTransform);
		break;
	case 1:
		mpLbc->opRaiseEvent(false, data, enEnemyTransform);
		break;
	}
}

void LoadBalancingListener::RaiseLocalPlayerInput(Car::CarControll input) {
	ExitGames::Common::Hashtable ev;
	ExitGames::Common::Hashtable hash;

	hash.put(ACC, input.accel);
	hash.put(BACK, input.back);
	hash.put(STEER, input.steering);
	hash.put(AX, input.aerealX);
	hash.put(AY, input.aerealY);
	hash.put(JUMP, input.jump);
	hash.put(BOOST, input.boost);
	hash.put(AIRR, input.airRoll);

	ev.put(key, hash);

	mpLbc->opRaiseEvent(false, ev, enInputs);
}

//opRaiseEventでイベントが送信されるとこの関数が呼ばれる
void LoadBalancingListener::customEventAction(int playerNr, nByte eventCode, const Object& eventContentObj)
{
	ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();

	//printf("Called Load Balancing Listener customEventAction\n");

	switch (eventCode) {
	case enInputs:
	{
		ExitGames::Common::Hashtable hashData;
		
		if (eventContent.getValue(key))
		{

			hashData = (ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent.getValue(key))).getDataCopy();


			if (hashData.getValue(ACC)) {
				m_enemyCon.accel = (ExitGames::Common::ValueObject<float>(hashData.getValue(ACC))).getDataCopy();
			}

			if (hashData.getValue(BACK))
				m_enemyCon.back = (ExitGames::Common::ValueObject<float>(hashData.getValue(BACK))).getDataCopy();

			if (hashData.getValue(STEER))
				m_enemyCon.steering = (ExitGames::Common::ValueObject<float>(hashData.getValue(STEER))).getDataCopy();

			if (hashData.getValue(AX))
				m_enemyCon.aerealX = (ExitGames::Common::ValueObject<float>(hashData.getValue(AX))).getDataCopy();

			if (hashData.getValue(AY))
				m_enemyCon.aerealY = (ExitGames::Common::ValueObject<float>(hashData.getValue(AY))).getDataCopy();

			if (hashData.getValue(JUMP))
				m_enemyCon.jump = (ExitGames::Common::ValueObject<float>(hashData.getValue(JUMP))).getDataCopy();

			if (hashData.getValue(BOOST))
				m_enemyCon.boost = (ExitGames::Common::ValueObject<bool>(hashData.getValue(BOOST))).getDataCopy();

			if (hashData.getValue(AIRR))
				m_enemyCon.airRoll = (ExitGames::Common::ValueObject<bool>(hashData.getValue(AIRR))).getDataCopy();
		}
	}
	break;
	case enEnemyTransform:
	{
		Object const* obj = eventContent.getValue("1");
		if (!obj)
			obj = eventContent.getValue((nByte)1);
		if (!obj)
			obj = eventContent.getValue(1);
		if (!obj)
			obj = eventContent.getValue(1.0);

		float x = 0; float y = 0; float z = 0;

		if (obj && obj->getDimensions() == 1 && obj->getSizes()[0] == 3)
		{
			float x = 0; float y = 0; float z = 0;
			if (obj->getType() == TypeCode::FLOAT)
			{
				float* data = ((ValueObject<float*>*)obj)->getDataCopy();
				x = (float)data[0];
				y = (float)data[1];
				z = (float)data[2];
				CVector3 npos;
				npos.x = x;
				npos.y = y;
				npos.z = z;

				printf("Get My Car Transform float x : %f, y : %f\n", x, y);
				m_localPlayerCar->ResetCar(npos);
			}
		}
	}
	break;
	case enMyTransform:
	{
		Object const* obj = eventContent.getValue("1");
		if (!obj)
			obj = eventContent.getValue((nByte)1);
		if (!obj)
			obj = eventContent.getValue(1);
		if (!obj)
			obj = eventContent.getValue(1.0);

		float x = 0; float y = 0; float z = 0;

		if (obj && obj->getDimensions() == 1 && obj->getSizes()[0] == 3)
		{
			float x = 0; float y = 0; float z = 0;
			if (obj->getType() == TypeCode::FLOAT)
			{
				float* data = ((ValueObject<float*>*)obj)->getDataCopy();
				x = (float)data[0];
				y = (float)data[1];
				z = (float)data[2];
				CVector3 npos;
				npos.x = x;
				npos.y = y;
				npos.z = z;

				printf("Get Enemy Car Transform x : %f, y : %f\n", x, y);
				m_onlinePlayerCar->ResetCar(npos);
			}
		}
	}
	break;
	}
}

void LoadBalancingListener::disconnectReturn(void)
{
	updateState();
	Console::get().writeLine(L"disconnected");
}

void LoadBalancingListener::createRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if (errorCode == ErrorCode::OK)
	{
		Console::get().writeLine(L"room has been created");
		afterRoomJoined(localPlayerNr);
	}
	else
		Console::get().writeLine(L"Warn: opCreateRoom() failed: " + errorString);
}

void LoadBalancingListener::joinOrCreateRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if (errorCode == ErrorCode::OK)
	{
		Console::get().writeLine(L"room has been entered");
		afterRoomJoined(localPlayerNr);
	}
	else
		Console::get().writeLine(L"Warn: opJoinOrCreateRoom() failed: " + errorString);
}

void LoadBalancingListener::joinRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if (errorCode == ErrorCode::OK)
	{
		Console::get().writeLine(L"game room has been successfully joined");
		afterRoomJoined(localPlayerNr);
	}
	else
		Console::get().writeLine(L"opJoinRoom() failed: " + errorString);
}

void LoadBalancingListener::joinRandomRoomReturn(int localPlayerNr, const Hashtable& gameProperties, const Hashtable& playerProperties, int errorCode, const JString& errorString)
{
	updateState();
	if (errorCode == ErrorCode::NO_MATCH_FOUND)
		createRoom();
	else if (errorCode == ErrorCode::OK)
	{
		Console::get().writeLine(L"game room has been successfully joined");
		afterRoomJoined(localPlayerNr);
	}
	else
		Console::get().writeLine(L"opJoinRandomRoom() failed: " + errorString);
}

void LoadBalancingListener::leaveRoomReturn(int errorCode, const JString& errorString)
{
	updateState();
	if (errorCode == ErrorCode::OK)
		Console::get().writeLine(L"game room has been successfully left");
	else
		Console::get().writeLine(L"opLeaveRoom() failed: " + errorString);
}

void LoadBalancingListener::gotQueuedReturn(void)
{
	updateState();
}

void LoadBalancingListener::joinLobbyReturn(void)
{
	Console::get().writeLine(L"joined lobby");
	updateState();
}

void LoadBalancingListener::leaveLobbyReturn(void)
{
	Console::get().writeLine(L"left lobby");
	updateState();
}

void LoadBalancingListener::onLobbyStatsUpdate(const JVector<LobbyStatsResponse>& res)
{
	Console::get().writeLine(L"===================== lobby stats update");
	for (unsigned int i = 0; i < res.getSize(); ++i)
		Console::get().writeLine(res[i].toString());

	// lobby stats request test
	JVector<LobbyStatsRequest> ls;
	ls.addElement(LobbyStatsRequest());
	ls.addElement(LobbyStatsRequest(L"AAA"));
	ls.addElement(LobbyStatsRequest(L"BBB"));
	ls.addElement(LobbyStatsRequest(L"CCC", LobbyType::DEFAULT));
	ls.addElement(LobbyStatsRequest(L"AAA", LobbyType::SQL_LOBBY));
	mpLbc->opLobbyStats(ls);
}

void LoadBalancingListener::onLobbyStatsResponse(const ExitGames::Common::JVector<LobbyStatsResponse>& res)
{
	Console::get().writeLine(L"===================== lobby stats response");
	for (unsigned int i = 0; i < res.getSize(); ++i)
		Console::get().writeLine(res[i].toString());
}

void LoadBalancingListener::onRoomListUpdate()
{
	const JVector<Room*>& rooms = mpLbc->getRoomList();
	JVector<JString> names(rooms.getSize());
	for (unsigned int i = 0; i < rooms.getSize(); ++i)
		names.addElement(rooms[i]->getName());
	//mpView->updateRoomList(names);
}

void LoadBalancingListener::onRoomPropertiesChange(const ExitGames::Common::Hashtable& changes)
{
}

void LoadBalancingListener::updateState()
{
	int state = mpLbc->getState();
	mpView->updateState(state, PeerStatesStr[state], state == PeerStates::Joined ? mpLbc->getCurrentlyJoinedRoom().getName() : JString());
}


void LoadBalancingListener::afterRoomJoined(int localPlayerNr)
{
	Console::get().writeLine(JString(L"afterRoomJoined: localPlayerNr=") + localPlayerNr);
	this->mLocalPlayerNr = localPlayerNr;
	MutableRoom& myRoom = mpLbc->getCurrentlyJoinedRoom();
	Hashtable props = myRoom.getCustomProperties();
}

void LoadBalancingListener::createRoom()
{
	JString name =
#ifdef __UNREAL__
		JString(L"UE-")
#else
		JString(L"native-")
#endif
		+ (rand() % 100);
	//Hashtable props;
	//props.put(L"m", mMap);
	//RoomOptions roomOptions(bool isVisible = true, bool isOpen = true, nByte maxPlayers = 2);
	//mpLbc->opCreateRoom(name, RoomOptions().setCustomRoomProperties(props));

	mpLbc->opCreateRoom(name, RoomOptions().setMaxPlayers(m_maxPlayer));
	Console::get().writeLine(L"Creating room " + name);
}

void LoadBalancingListener::connectReturn(int errorCode, const JString& errorString, const JString& region, const JString& cluster)
{
	updateState();
	if (errorCode == ErrorCode::OK)
	{
		Console::get().writeLine(L"connected to cluster " + cluster + L" of region " + region);
		mpLbc->opJoinRandomRoom(Hashtable(), m_maxPlayer);
	}
	else
		Console::get().writeLine(JString(L"Warn: connect failed ") + errorCode + L" " + errorString);
}

void LoadBalancingListener::service()
{
	unsigned long t = GETTIMEMS();
	if ((t - lastUpdateTime) > PLAYER_UPDATE_INTERVAL_MS)
	{
		lastUpdateTime = t;
		if (mpLbc->getState() == PeerStates::Joined) {
		}
	}
}

JString LoadBalancingListener::GetState() {
	return PeerStatesStr[mpLbc->getState()];
}
