#pragma once
#include "OutputListener.h"

class ConsoleImplementation
{
public:
	void write(const ExitGames::Common::JString& str)
	{
		OutputDebugStringW(str.cstr());
	}

	void writeLine(const ExitGames::Common::JString& str)
	{
		write(str);
	}
};


class Console : public OutputListener
{
public:
	Console();
	~Console();
	void update(void);
	void write(const ExitGames::Common::JString& str);
	void writeLine(const ExitGames::Common::JString& str = ExitGames::Common::JString());
	static Console& get(void);
private:
	ConsoleImplementation* mpImp = nullptr;
};

