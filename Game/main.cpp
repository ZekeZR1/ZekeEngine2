#include "stdafx.h"
#include "Game.h"
#include "Title.h"
#include "OnlineMatch.h"
#include "TestScene.h"

signed WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow) {

	//Console
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);

	//Initialize
	EngineParam  eParam;
	eParam.hInstance = hInstance;
	eParam.hPrevInstance = hPrevInstance;
	eParam.lpCmdLine = lpCmdLine;
	eParam.nCmdShow = nCmdShow;
	eParam.screenWidth = 1280;
	eParam.screenHeight = 720;

	//Start Main Roop
	if (Engine().Init(eParam)) {
		//Camera
		MainCamera().SetTarget(CVector3::Zero());
		MainCamera().SetPosition({ 0,4,-4 });
		MainCamera().SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		MainCamera().SetNear(0.1f);
		MainCamera().SetFar(1000.0f);
		MainCamera().Update();
		//2d
		MainCamera2D().SetTarget(CVector3::Zero());
		MainCamera2D().SetPosition({ 0.0f, 0.0f, -10.0f });
		MainCamera2D().SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		MainCamera2D().SetNear(1.f);
		MainCamera2D().SetFar(3000.0f);
		MainCamera2D().Update();
		//bullet physics debug drawing flag
		PhysicsWorld().SetDebugDraw(false);

		NewGO<OnlineMatch>(0);
		//NewGO<Title>(0, "Title");
		Engine().GameRoop();
	}

	//Release
	Engine().Final();
}
