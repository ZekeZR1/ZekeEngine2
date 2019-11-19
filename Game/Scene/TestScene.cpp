#include "stdafx.h"
#include "TestScene.h"
#include "../GameObjects/GameCamera.h"
#include "../GameObjects/Stage.h"
#include "..//..//ZekeEngine2/graphics/Media/MediaPlayer.h"

bool TestScene::Start() {
	auto wnd = FindWindow(NULL,"Game");
	CPlayer::CreateInstance(wnd, wnd, &g_pPlayer);

	return true;
}

void TestScene::OnDestroy() {
	if (g_pPlayer)
	{
		g_pPlayer->Shutdown();
		SafeRelease(&g_pPlayer);
	}
}

void TestScene::Update() {

}