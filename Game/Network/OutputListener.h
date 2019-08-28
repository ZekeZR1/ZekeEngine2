#pragma once
#include "Common-cpp/inc/Common.h"

class OutputListener : public virtual ExitGames::Common::BaseListener
{
public:
	OutputListener() {};
	virtual ~OutputListener() {};
	virtual void write(const ExitGames::Common::JString& str) = 0;
	virtual void writeLine(const ExitGames::Common::JString& str = ExitGames::Common::JString()) = 0;
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& str)
	{
		OutputDebugStringW((str + L"\n").cstr());
	}
};