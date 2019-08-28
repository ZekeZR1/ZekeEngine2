#include "stdafx.h"
#include "OutputListener.h"
#include "Console.h"
#include "NetworkLogic.h"

Console::Console()
{
}

Console::~Console()
{
}

void Console::update()
{
}

void Console::write(const ExitGames::Common::JString& str)
{
	mpImp->write(str);
}

void Console::writeLine(const ExitGames::Common::JString& str)
{
	mpImp->writeLine(str);
}

Console& Console::get(void)
{
	static Console console;
	return console;
}
