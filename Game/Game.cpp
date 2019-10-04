#include "stdafx.h"
#include "Game.h"
#include "ScoreManager.h"
#include "Vehicle.h"
#include "Ball.h"
#include "Stage.h"
#include "GameCamera.h"
#include "Car.h"
#include "Network/NetworkLogic.h"
#include "Network/LoadBalancingListener.h"
#include "Result.h"

bool Game::Start() {
	m_stage = NewGO<Stage>(0);
	m_ball = NewGO<Ball>(0,"BallChan");
	m_myCar = NewGO<Car>(0,"MyCar");
	m_myCar->ResetCar({ 0,2,-10 });

	m_enemyCar = NewGO<Car>(0, "EnemyCar");
	m_enemyCar->ResetCar({ 0,2,10 });

	m_gameCamera = NewGO<GameCamera>(0);
	m_scoreManager = NewGO < ScoreManager>(0,"ScoreManager");

	NetworkLogic::GetInstance().GetLBL()->SetCars(m_myCar, m_enemyCar);

	m_NetworkLagTime = NetworkLogic::GetInstance().GetLBL()->GetLagAve();

	auto lag = NetworkLogic::GetInstance().GetLBL()->GetLag();
	float lg  = (float)lag / 1000.f;
	printf("lag %d\n", lag);
	m_NetworkLagTime = lg;

	return true;
}

void Game::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_ball);
	DeleteGO(m_gameCamera);
	DeleteGO(m_myCar);
	DeleteGO(m_enemyCar);
	DeleteGO(m_scoreManager);

	NetworkLogic::GetInstance().Disconnect();
}

void Game::Update() {
	NetworkLogic::GetInstance().Update();

	NetworkLogic::GetInstance().GetLBL()->RaiseCurrentLocalTime();

	m_raiseTimer++;

	int lpn = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerNumber();
	int opn = NetworkLogic::GetInstance().GetLBL()->GetEnemyPlayerNumber();

	if (Pad(0).IsTrigger(enButtonStart)) {
		m_ball->ResetBall();
		if (lpn < opn) {
			printf("Im Host\n");
		}
		printf("Lag %d\n", NetworkLogic::GetInstance().GetLBL()->GetLag());
		//NetworkLogic::GetInstance().GetLBC()->()
	}


	////トルクとフォース情報のみで車を動かす実験
	//{
	//	SetInputs();
	//	m_myCar->SetCarInput(m_carCon);

	//	auto lv = m_myCar->GetRayCastVehicle()->getRigidBody()->getLinearVelocity();
	//	auto agv = m_myCar->GetRayCastVehicle()->getRigidBody()->getAngularVelocity();

	//	//printf("lv %f, %f, %f\n", lv.getX(), lv.getY(), lv.getZ());

	//	m_enemyCar->GetRayCastVehicle()->getRigidBody()->setLinearVelocity(lv);
	//	m_enemyCar->GetRayCastVehicle()->getRigidBody()->setAngularVelocity(agv);
	//}

	//Host
	if (lpn < opn) {
		
		m_myCar->SetHostCarFlag(true);
		m_enemyCar->SetHostCarFlag(true);

		SetInputs();
		m_inputDataQueue.push(m_carCon);

		//RaiseInputs();
		RaiseCarVelocitys();

		//if (m_lagFixTimer >= m_NetworkLagTime) {
			if (true) {
				m_myCar->SetCarInput(m_inputDataQueue.front());
				m_inputDataQueue.pop();
			}
			else {
				m_lagFixTimer += IGameTime().GetFrameDeltaTime();
			}
		//}
		//m_myCar->SetCarInput(cc);

		auto eci = NetworkLogic::GetInstance().GetLBL()->GetEnemeyCarInputs();
		m_enemyCar->SetCarInput(eci);

		if (m_raiseTimer == 60) {
			RaiseCarTransform();
			m_raiseTimer = 0;
		}
	}
	else {
		SetInputs();
		RaiseInputs();

		auto ecav = NetworkLogic::GetInstance().GetLBL()->GetEnemyCarAnguraVelocity();
		auto eclv = NetworkLogic::GetInstance().GetLBL()->GetEnemyCarLinearVelocity();
		m_enemyCar->GetRayCastVehicle()->getRigidBody()->setLinearVelocity(eclv);
		m_enemyCar->GetRayCastVehicle()->getRigidBody()->setAngularVelocity(ecav);


		auto mcav = NetworkLogic::GetInstance().GetLBL()->GetMyCarAnguraVelocity();
		auto mclv = NetworkLogic::GetInstance().GetLBL()->GetMyCarLinearVelocity();
		m_myCar->GetRayCastVehicle()->getRigidBody()->setLinearVelocity(mclv);
		m_myCar->GetRayCastVehicle()->getRigidBody()->setAngularVelocity(mcav);


		//auto eci = NetworkLogic::GetInstance().GetLBL()->GetEnemeyCarInputs();

		//m_enemyCar->SetCarInput(eci);

		//m_myCar->SetCarInput(eci);

		//NetworkLogic::GetInstance().GetLBL()->GetGameTime();
	}

	////Test
	//{
	//	SetInputs();
	//	m_inputDataQueue.push(m_carCon);
	//	

	//	if (m_lagFixTimer >= m_NetworkLagTime) {
	//		m_myCar->SetCarInput(m_inputDataQueue.front());
	//		m_inputDataQueue.pop();
	//	}
	//	else {
	//		m_lagFixTimer += IGameTime().GetFrameDeltaTime();
	//	}
	//}

	auto pos = m_ball->GetPosition();
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

	if(m_scoreManager->IsGameOver()){
		//DeleteGO(this);
		//NewGO<Result>(0, "ResultScene");
	}
}

void Game::SetInputs() {
	m_carCon.accel = Pad(0).GetRTrigger();
	m_carCon.back = Pad(0).GetLTrigger();
	m_carCon.steering = Pad(0).GetLStickXF();
	m_carCon.aerealX = Pad(0).GetLStickXF();
	m_carCon.aerealY = Pad(0).GetLStickYF();
	m_carCon.jump = Pad(0).IsTrigger(enButtonA);
	m_carCon.boost = Pad(0).IsPress(enButtonB);
	m_carCon.airRoll = Pad(0).IsPress(enButtonRB1);
}

void Game::RaiseCarVelocitys() {
	auto lv = m_myCar->GetRayCastVehicle()->getRigidBody()->getLinearVelocity();
	auto agv = m_myCar->GetRayCastVehicle()->getRigidBody()->getAngularVelocity();

	NetworkLogic::GetInstance().GetLBL()->RaiseForceAndTorque(lv,agv,0);

	 lv = m_enemyCar->GetRayCastVehicle()->getRigidBody()->getLinearVelocity();
	 agv = m_enemyCar->GetRayCastVehicle()->getRigidBody()->getAngularVelocity();
	NetworkLogic::GetInstance().GetLBL()->RaiseForceAndTorque(lv, agv, 1);

}

void Game::RaiseCarTransform() {
	NetworkLogic::GetInstance().GetLBL()->RaiseCarTransform(m_myCar->GetPosition(), m_myCar->GetRotation(), 0);

	NetworkLogic::GetInstance().GetLBL()->RaiseCarTransform(m_enemyCar->GetPosition(), m_enemyCar->GetRotation(), 1);
}

void Game::RaiseInputs() {
	NetworkLogic::GetInstance().GetLBL()->RaiseLocalPlayerInput(m_carCon);
}