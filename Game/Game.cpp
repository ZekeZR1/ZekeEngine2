#include "stdafx.h"
#include "Game.h"
#include "ScoreManager.h"
#include "Vehicle.h"
#include "Ball.h";
#include "Stage.h"
#include "GameCamera.h"
#include "Car.h"

bool Game::Start() {
	m_stage = NewGO<Stage>(0);
	m_ball = NewGO<Ball>(0,"BallChan");
	m_car = NewGO<Car>(0,"Car");
	m_car->ResetCar();
	//m_car->SetChassisPositionFix(CVector3(0.f,-0.3f,0.f ));
	m_gameCamera = NewGO<GameCamera>(0);
	m_scoreManager = NewGO < ScoreManager>(0,"ScoreManager");
	return true;
}

void Game::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_ball);
	DeleteGO(m_gameCamera);
	DeleteGO(m_car);
	DeleteGO(m_scoreManager);
}

void Game::Update() {
	if (Pad(0).IsTrigger(enButtonStart)) {
		m_ball->ResetBall();
	}
	auto pos = m_ball->GetPosition();
	//m_gameCamera->SetTarget(m_car.GetPosition());
	m_gameCamera->SetTarget(pos);
	CVector3 cameraPos = CVector3::Zero();
	auto carToBallVec = m_ball->GetPosition() - m_car->GetPosition();
	carToBallVec.Normalize();
	cameraPos = m_car->GetPosition() - (carToBallVec * 8);
	if (cameraPos.y < 3.f)
		cameraPos.y = 3.f;
	else
		cameraPos.y = m_car->GetPosition().y + 3.f;
	m_gameCamera->SetCameraPosition(cameraPos);
}
