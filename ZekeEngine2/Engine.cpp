#include "stdafx.h"
#include "Engine.h"
#include "FPSCounter.h"

CFPSCounter* FPS = nullptr;

int nNotch = 0;

Engine::Engine()
{
}


Engine::~Engine()
{
}

void Engine::Init(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow) {
	//SetCurrentDirectory("Resource");
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, L"Game");
	IGameObjectManager().Init(32);
	FPS = new CFPSCounter(10);
}

void Engine::Release() {
	delete g_graphicsEngine;
	delete FPS;
}

void Engine::Update() {
	for (auto& pad : g_pad) {
		pad.Update();
	}

	Mouse::UpdateMouseInput();

	IGameObjectManager().Execute();
#if _DEBUG
	FPS->Draw();
#endif
}

void Engine::GameRoop() {
	while (DispatchWindowMessage()) {
		Update();
	}
}

void Engine::Final() {
	Release();
}

