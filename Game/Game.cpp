#include "stdafx.h"
#include "Game.h"
#include "Vehicle.h"
#include "Stage.h"
#include "GameCamera.h"

bool Game::Start() {
	m_vehicle.init();
	NewGO<Stage>(0);
	m_gameCamera = NewGO<GameCamera>(0);
	return true;
}

void Game::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_gameCamera);
}

void Game::Update() {
	m_vehicle.stepSimulation();
	m_vehicle.KeyUpdate();
}
