#include "stdafx.h"
#include "TestScene.h"
#include "../GameObjects/GameCamera.h"
#include "../GameObjects/Stage.h"

bool TestScene::Start() {
	m_stage = NewGO<Stage>(0);
	m_myCar = NewGO<Car>(0, "MyCar");
	m_myCar->ResetCar({ 0,2,-10 });
	m_enemyCar = NewGO<Car>(0, "EnemyCar");
	m_enemyCar->ResetCar({ 0,2,10 });
	m_gameCamera = NewGO<GameCamera>(0);
	return true;
}

void TestScene::OnDestroy() {

}

void TestScene::Update() {
	m_carCon.accel = Pad(0).GetRTrigger();
	m_carCon.back = Pad(0).GetLTrigger();
	m_carCon.steering = Pad(0).GetLStickXF();
	m_carCon.aerealX = Pad(0).GetLStickXF();
	m_carCon.aerealY = Pad(0).GetLStickYF();
	m_carCon.jump = Pad(0).IsTrigger(enButtonA);
	m_carCon.boost = Pad(0).IsPress(enButtonB);
	m_carCon.airRoll = Pad(0).IsPress(enButtonRB1);
	m_myCar->SetCarInput(m_carCon);

	auto lv = m_myCar->GetRayCastVehicle()->getRigidBody()->getLinearVelocity();
	auto agv = m_myCar->GetRayCastVehicle()->getRigidBody()->getAngularVelocity();

	//printf("lv %f, %f, %f\n", lv.getX(), lv.getY(), lv.getZ());

	//m_enemyCar->GetRayCastVehicle()->getRigidBody()->setLinearVelocity(lv);
	//m_enemyCar->GetRayCastVehicle()->getRigidBody()->setAngularVelocity(agv);

}