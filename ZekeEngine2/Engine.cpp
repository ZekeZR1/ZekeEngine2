#include "stdafx.h"
#include "Engine.h"
#include "Engine/FPSCounter.h"

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
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
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

	if (m_network != nullptr) {
		m_network->Update();
	}

	IGameObjectManager().Execute();
#if _DEBUG
	FPS->Draw();
#endif
}

void Engine::CreateNetworkSystem() {
	if (m_network == nullptr) {
		m_network = new NetworkLogic;
		m_network->Start();
	}
}

void Engine::DestroyNetworkSystem() {
	if (m_network != nullptr) {
		m_network->Disconnect();
		delete m_network;
		m_network = nullptr;
	}
}

void Engine::GameRoop() {
	while (DispatchWindowMessage()) {
		Update();
	}
}

void Engine::Final() {
	Release();
}

