#pragma once
#include "stdafx.h"

struct EngineParam {
	HINSTANCE  hInstance;
	HINSTANCE hPrevInstance;
	LPWSTR lpCmdLine;
	int screenWidth;
	int screenHeight;
	int nCmdShow;
};