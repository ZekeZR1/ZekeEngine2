#include "stdafx.h"
#include "Game.h"

signed WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow) {
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
		MainCamera().SetPosition({ 0,0,-500 });
		MainCamera().SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
		MainCamera().SetNear(0.1f);
		MainCamera().SetFar(50000.0f);
		MainCamera().Update();
		//2d
		MainCamera2D().SetTarget(CVector3::Zero());
		MainCamera2D().SetPosition({ 0.0f, 0.0f, -10.0f });
		MainCamera2D().SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		MainCamera2D().SetNear(0.1f);
		MainCamera2D().SetFar(1000.0f);
		MainCamera2D().Update();
		//bullet physics debug drawing flag
		PhysicsWorld().SetDebugDraw(true);

		NewGO<Game>(0, "Game");
		Engine().GameRoop();
	}

	//Release
	Engine().Final();
}
