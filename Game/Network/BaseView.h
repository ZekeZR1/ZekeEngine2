#pragma once
#include "Common-cpp/inc/Common.h"
class BaseView
{
public:
	virtual ~BaseView(void) {}
	virtual void updateState(int state, const ExitGames::Common::JString& stateStr, const ExitGames::Common::JString& joinedRoomName) = 0;
};

