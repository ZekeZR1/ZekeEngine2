#include "stdafx.h"
#include "Training.h"
#include "..//GameObjects//Ball.h"
#include "../GameObjects/ScoreManager.h"
#include "../GameObjects/Stage.h"
#include "../GameObjects/GameCamera.h"
#include "../GameObjects/Car.h"
#include "Result.h"
#include "..//GameObjects/Goal.h"
#include "Title.h"

bool Training::Start() {
	m_stage = NewGO<Stage>(0);
	m_ball = NewGO<Ball>(0, "BallChan");
	m_myCar = NewGO<Car>(0, "MyCar");
	m_myCar->ResetCar(m_CarInitPos);
	m_gameCamera = NewGO<GameCamera>(0);
	//m_scoreManager = NewGO < ScoreManager>(0, "ScoreManager");
	PhysicsWorld().GetDynamicWorld()->getSimulationIslandManager()->setSplitIslands(false);
	return true;
}

void Training::OnDestroy() {
	DeleteGO(m_stage);
	DeleteGO(m_ball);
	DeleteGO(m_gameCamera);
	DeleteGO(m_myCar);
	//DeleteGO(m_scoreManager);
}

void Training::Update() {
	IGameObjectManager().SetShadowCameraPosAndTarget({ m_myCar->GetPosition().x,m_myCar->GetPosition().y + 50,m_myCar->GetPosition().z }, { m_myCar->GetPosition().x,m_myCar->GetPosition().y,m_myCar->GetPosition().z });

	if (Pad(0).IsTrigger(enButtonStart)) {
		m_ball->ResetBall();
		m_myCar->ResetCar(m_CarInitPos);
	}

	m_gameCamera->SetBallPos(m_ball->GetPosition());
	m_gameCamera->SetLocalCar(m_myCar);

	m_myCar->SetHostCarFlag(true);

	m_carCon.accel = Pad(0).GetRTrigger();
	m_carCon.back = Pad(0).GetLTrigger();
	m_carCon.steering = Pad(0).GetLStickXF();
	m_carCon.aerealX = Pad(0).GetLStickXF();
	m_carCon.aerealY = Pad(0).GetLStickYF();
	m_carCon.jump = Pad(0).IsTrigger(enButtonA);
	m_carCon.boost = Pad(0).IsPress(enButtonB);
	m_carCon.airRoll = Pad(0).IsPress(enButtonRB1);

	m_myCar->SetCarInput(m_carCon);

	if (m_stage->GetGoalObject() != nullptr) {
		if (m_stage->GetGoalObject()->IsScored()) {
			m_myCar->ResetCar(m_CarInitPos);
			m_ball->ResetBall();
			m_stage->GetGoalObject()->SetScoredFlag(false);
		}
	}

	if (ZKeyBoard().GetStateTracker().pressed.Escape) {
		DeleteGO(this);
		NewGO<Title>(0, "TitleScene");
	}
}
