#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "..//GameObjects/Car.h"
#include "..//GameObjects/Stage.h"
#include "..//GameObjects/TitleMenu.h"

bool Title::Start() {
	m_menu = NewGO<TitleMenu>(0);
	m_stage = NewGO<Stage>(0);
	m_myCar = NewGO<Car>(0, "MyCar");
	m_myCar->ResetCar(CVector3::Zero());
	MainCamera().SetPosition({ -2,2,4 });
	MainCamera().Update();

	return true;
}

void Title::OnDestroy() {

}

void Title::Update() {

}