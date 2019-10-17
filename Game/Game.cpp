#include "stdafx.h"
#include "Game.h"
#include "Goal.h"
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
	m_myCar->ResetCar(m_localCarInitPos);
	m_enemyCar = NewGO<Car>(0, "EnemyCar");
	m_enemyCar->ResetCar(m_onlineCarInitPos);
	m_gameCamera = NewGO<GameCamera>(0);
	m_scoreManager = NewGO < ScoreManager>(0,"ScoreManager");

	NetworkLogic::GetInstance().GetLBL()->SetCars(m_myCar, m_enemyCar);

	m_NetworkLagTime = NetworkLogic::GetInstance().GetLBL()->GetLagAve();

	auto lag = NetworkLogic::GetInstance().GetLBL()->GetLag();
	float lg  = (float)lag / 1000.f;
	//printf("lag %d\n", lag);
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

	if (Pad(0).IsTrigger(enButtonStart)) {
		printf("car pos x %f, z , %f", m_myCar->GetPosition().x, m_myCar->GetPosition().z);
	}

	IGameObjectManager().SetShadowCameraPosAndTarget({ m_myCar->GetPosition().x,m_myCar->GetPosition().y + 50,m_myCar->GetPosition().z }, { m_myCar->GetPosition().x,m_myCar->GetPosition().y,m_myCar->GetPosition().z });

	NetworkLogic::GetInstance().Update();

	NetworkLogic::GetInstance().GetLBL()->RaiseCurrentLocalTime();

	m_raiseTimer++;

	int lpn = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerNumber();
	int opn = NetworkLogic::GetInstance().GetLBL()->GetEnemyPlayerNumber();

	if (Pad(0).IsTrigger(enButtonStart)) {
		m_ball->ResetBall();
		m_myCar->ResetCar(m_localCarInitPos);
		m_enemyCar->ResetCar(m_onlineCarInitPos);
		if (lpn < opn) {
			printf("Im Host\n");
		}
		printf("Lag %d\n", NetworkLogic::GetInstance().GetLBL()->GetLag());

	}

	//Host
	//if (lpn < opn) {
	if (true) {

		m_myCar->SetHostCarFlag(true);
		m_enemyCar->SetHostCarFlag(true);

		SetInputs();
		m_inputDataQueue.push(m_carCon);

		{//goal
			auto pos = m_ball->GetPosition();
			if (pos.z <= -175 or pos.z >= 175) {
				if (pos.z < 0) {
					m_scoreManager->Goal(ScoreManager::enOrangeTeam);
				}
				if (pos.z > 0)
				{
					m_scoreManager->Goal(ScoreManager::enBlueTeam);
				}
				printf("Goal");
				RaiseGameScore();
				m_myCar->ResetCar(m_localCarInitPos);
				m_enemyCar->ResetCar(m_onlineCarInitPos);
				m_ball->ResetBall();
			}
		}

		//RaiseInputs();
		RaiseCarVelocitys();

		RaiseBallTransform();

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

		//if (m_raiseTimer ==180) {
			RaiseCarTransform();
			m_raiseTimer = 0;
		//}
	}
	else {
		SetInputs();
		RaiseInputs();

		auto lpos = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerView().pos;
		auto lrot = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerView().rot;
		auto opos = NetworkLogic::GetInstance().GetLBL()->GetOnlinePlayerView().pos;
		auto orot = NetworkLogic::GetInstance().GetLBL()->GetOnlinePlayerView().rot;

		auto dif = m_myCar->GetPosition() - lpos;
		float fixDist = 5.f;
		if (abs(dif.Length()) > fixDist) {
			m_myCar->SetTransform(lpos, lrot);
		}

		dif = m_enemyCar->GetPosition() - opos;
		if (abs(dif.Length()) > fixDist) {
			m_enemyCar->SetTransform(opos, orot);
		}


		auto ecav = NetworkLogic::GetInstance().GetLBL()->GetOnlinePlayerView().angularVelocity;
		auto eclv = NetworkLogic::GetInstance().GetLBL()->GetOnlinePlayerView().linearVelocity;
		m_enemyCar->GetRayCastVehicle()->getRigidBody()->setLinearVelocity(eclv);
		m_enemyCar->GetRayCastVehicle()->getRigidBody()->setAngularVelocity(ecav);


		auto mcav = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerView().angularVelocity;
		auto mclv = NetworkLogic::GetInstance().GetLBL()->GetLocalPlayerView().linearVelocity;
		m_myCar->GetRayCastVehicle()->getRigidBody()->setLinearVelocity(mclv);
		m_myCar->GetRayCastVehicle()->getRigidBody()->setAngularVelocity(mcav);

		auto bp = NetworkLogic::GetInstance().GetLBL()->GetBallPos();
		auto br = NetworkLogic::GetInstance().GetLBL()->GetBallRot();

		m_ball->SetTransform(bp,br);

		auto bts = NetworkLogic::GetInstance().GetLBL()->GetBlueTeamScore();
		auto ots = NetworkLogic::GetInstance().GetLBL()->GetOrangeTeamScore();

		m_scoreManager->SetPoint(bts, ots);

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

	m_gameCamera->SetBallPos(m_ball->GetPosition());
	m_gameCamera->SetLocalCar(m_myCar);

	if(m_scoreManager->IsGameOver()){
		//DeleteGO(this);
		//NewGO<Result>(0, "ResultScene");
	}
}

void Game::PostUpdate() {
	//IGameObjectManager().GetShadowMap()->UpdateFromLightDirection({ m_myCar->GetPosition().x,100,m_myCar->GetPosition().z },
//		{ m_myCar->GetPosition().x,0,m_myCar->GetPosition().z }
//	);
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

void Game::RaiseBallTransform() {
	NetworkLogic::GetInstance().GetLBL()->RaiseBallTransform(m_ball->GetPosition(),m_ball->GetRotation());
}


void Game::RaiseBallVelocity() {

}

void Game::RaiseGameScore() {
	NetworkLogic::GetInstance().GetLBL()->RaiseGameScore(m_scoreManager->GetBluePoint(),m_scoreManager->GetOrangePoint());
}

void Game::RaiseGameTime() {

}