#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "..//GameObjects/Car.h"
#include "..//GameObjects/Stage.h"
#include "..//GameObjects/TitleMenu.h"
#include "Training.h"
#include "Scene/OnlineMatch.h"

bool Title::Start() {
	m_menu = NewGO<TitleMenu>(0);
	m_stage = NewGO<Stage>(0);
	m_myCar = NewGO<Car>(0, "MyCar");
	m_myCar->ResetCar(CVector3::Zero());

	MainCamera().SetPosition({ -2,2,4 });
	MainCamera().SetTarget(CVector3::Zero());
	MainCamera().Update();
	MainCamera().Update();

	return true;
}

void Title::OnDestroy() {
	DeleteGO(m_menu);
	DeleteGO(m_stage);
	DeleteGO(m_myCar);
}

void Title::Update() {
	
	auto smenu = m_menu->GetSelectiongMenu();
	if (Pad(0).IsTrigger(enButtonA)) {
		switch (smenu) {
		case TitleMenu::enOnlineMatch:
			NewGO<OnlineMatch>(0);
			break;
		case TitleMenu::enTraining:
			NewGO<Training>(0, "TrainingScene");
			DeleteGO(this);
			break;
		}
	}
	
}