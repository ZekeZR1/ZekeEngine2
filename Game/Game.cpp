#include "stdafx.h"
#include "Game.h"
#include "ScoreManager.h"
#include "Vehicle.h"
#include "Ball.h";
#include "Stage.h"
#include "GameCamera.h"
#include "Car.h"
#include "Network/NetworkLogic.h"
#include "Result.h"

bool Game::Start() {
	m_stage = NewGO<Stage>(0);
	m_ball = NewGO<Ball>(0,"BallChan");
	m_myCar = NewGO<Car>(0,"Car");
	m_myCar->ResetCar({ 0,2,-10 });

	m_enemyCar = NewGO<Car>(0, "EnemyCar");
	m_enemyCar->ResetCar({ 0,2,10 });
	//m_car->SetChassisPositionFix(CVector3(0.f,-0.3f,0.f ));
	m_gameCamera = NewGO<GameCamera>(0);
	m_scoreManager = NewGO < ScoreManager>(0,"ScoreManager");
	return true;
}

void Game::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_ball);
	DeleteGO(m_gameCamera);
	DeleteGO(m_myCar);
	DeleteGO(m_enemyCar);
	DeleteGO(m_scoreManager);

	//TODO  : ƒlƒbƒg‘Îí
	//NetSystem().DestroyNetworkSystem();
}

void Game::Update() {
	if (Pad(0).IsTrigger(enButtonStart)) {
		m_ball->ResetBall();
	}

	auto pos = m_ball->GetPosition();
	//m_gameCamera->SetTarget(m_car.GetPosition());
	m_gameCamera->SetTarget(pos);
	CVector3 cameraPos = CVector3::Zero();
	auto carToBallVec = m_ball->GetPosition() - m_myCar->GetPosition();
	carToBallVec.Normalize();
	cameraPos = m_myCar->GetPosition() - (carToBallVec * 8);
	if (cameraPos.y < 3.f)
		cameraPos.y = 3.f;
	else
		cameraPos.y = m_myCar->GetPosition().y + 3.f;
	m_gameCamera->SetCameraPosition(cameraPos);

	//TODO tyantoshite
	auto lbl = NetSystem().GetNetworkLogic().GetLBL();
	if (lbl != nullptr) {
		NetSystem().GetNetworkLogic().GetLBL()->RaiseMyCarTransform(m_myCar->GetPosition(), m_myCar->GetRotation());
	}

	if(m_scoreManager->IsGameOver()){
		DeleteGO(this);
		NewGO<Result>(0, "ResultScene");
	}
}
