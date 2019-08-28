#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start() {
	return true;
}

void Title::OnDestroy() {

}

void Title::Update() {
	if (Pad(0).IsTrigger(enButtonA)) {
		NewGO<Game>(0);
		DeleteGO(this);
	}
}