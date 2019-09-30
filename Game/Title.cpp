#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start() {
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init(L"Assets/sprite/title.dds", 1280, 720);
	return true;
}

void Title::OnDestroy() {
	DeleteGO(m_spriteRender);
}

void Title::Update() {
	if (Pad(0).IsTrigger(enButtonA)) {
		NewGO<Game>(0);
		DeleteGO(this);
	}
}