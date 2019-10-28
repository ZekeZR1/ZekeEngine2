#include "stdafx.h"
#include "Result.h"
#include "Title.h"

bool Result::Start() {
	return true;
}

void Result::OnDestroy() {

}

void Result::Update() {
	if (Pad(0).IsTrigger(enButtonA)) {
		DeleteGO(this);
		NewGO<Title>(0,"TitleScene");
	}
}

