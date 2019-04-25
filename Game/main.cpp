#include "stdafx.h"
//#include "Game.h"

signed WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow) {
	//Initialize
	Engine::IEngine().Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	//NewGO<Game>(0,"Game");
	//camera
	camera3d = new Camera;
	camera3d->SetTarget(CVector3::Zero());
	camera3d->SetPosition({ 0,0,-100 });
	camera3d->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
	camera3d->SetNear(0.1f);
	camera3d->SetFar(50000.0f);
	camera3d->Update();
	//2d
	camera2d = new Camera;
	camera2d->SetTarget(CVector3::Zero());
	camera2d->SetPosition({ 0.0f, 0.0f, -10.0f });
	camera2d->SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	camera2d->SetNear(0.1f);
	camera2d->SetFar(1000.0f);
	camera2d->Update();

	g_physics.SetDebugDraw(true);
	//SkinModelRender* sm = NewGO<SkinModelRender>(0, "sm");
	//sm->Init(L"Assets/modelData/tesEnemy3.cmo");
	//sm->SetPosition(CVector3::Zero());
	//sm->SetScale({ 0.001f,0.001f,0.001f });
	//MainRoop
	Engine::IEngine().GameRoop();
	//Release
	Engine::IEngine().Final();
}
