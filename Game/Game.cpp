#include "stdafx.h"
#include "Game.h"
#include "Vehicle.h"
#include "Stage.h"
#include "GameCamera.h"

bool Game::Start() {
	NewGO<Stage>(0);
	m_car.SetChassisPositionFix(CVector3(0.f,0.8f,-0.f ));
	m_gameCamera = NewGO<GameCamera>(0);
	return true;
}

void Game::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_gameCamera);
}

void Game::Update() {
	m_gameCamera->SetTarget(m_car.GetPosition());
	m_car.stepSimulation();
	m_car.buttonUpdate();
	//m_vehicle.stepSimulation();
	//m_vehicle.KeyUpdate();
}
