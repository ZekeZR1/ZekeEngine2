#include "stdafx.h"
//#include "Game.h"

signed WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow) {
	//Initialize
	Engine::IEngine().Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	NewGO<Game>(0,"Game");
	g_physics.SetDebugDraw(true);
	SkinModelRender* sm = NewGO<SkinModelRender>(0, "sm");
	sm->Init(L"Assets/modelData/tesEnemy3.cmo");
	sm->SetPosition(CVector3::Zero());
	sm->SetScale({ 0.001f,0.001f,0.001f });
	MainRoop
	Engine::IEngine().GameRoop();
	Release
	Engine::IEngine().Final();
}
