#include "stdafx.h"
#include "CarState.h"
#include "Car.h"

//地上状態
void OnGroundState::Enter(Car* car) {

}

CarState* OnGroundState::Update(Car* car) {
	auto LStick = Pad(0).GetLStickXF();
	auto R2Trigger = Pad(0).GetRTrigger();
	auto L2Trigger = Pad(0).GetLTrigger();
	auto speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();

	float steeringClamp = 0.2f; //ステアリング制限
	float defaultBreakingForce = 10.f;

	//ステアリング
	{
		const float defaultSteerringClamp = 0.3;
		float steering = LStick;
		float speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();
		//値を小さく設定するほど高速で曲がりにくくなります。
		static float clampParam = 7.f;
		if (speed > 0) {
			steeringClamp = clampParam / speed;
		}
		if (steeringClamp > defaultSteerringClamp)
			steeringClamp = defaultSteerringClamp;
		if (steering > steeringClamp)
			steering = steeringClamp;
		if (steering < -steeringClamp)
			steering = -steeringClamp;

		car->SetSteering(steering);
	}

	//エンジンパワー
	{
		float engineForce = 0.f;
		//前進
		auto frontForce = R2Trigger * engineParam;
		if (car->GetRayCastVehicle()->getCurrentSpeedKmHour() > normalMaxSpeed)
			frontForce = 0;
		//printf("speed %f ... font force %f\n", speed,frontForce);
		//後退
		auto backForce = L2Trigger * engineParam;

		car->SetEngineForce(frontForce - backForce);

		if (R2Trigger > 0.f or L2Trigger > 0.f) {
			car->SetBreakingForce(0.f);
		}
		else {
			car->SetBreakingForce(defaultBreakingForce);
		}
	}

	//ブースト
	if (Pad(0).IsPress(enButtonB)) {
		static const int boostParam = 20000;
		auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
		auto forward = car->GetRayCastVehicle()->getForwardVector();
		if (speed < boostMaxSpeed)
			rigidbody->applyCentralForce(forward * boostParam);
	}
	//ジャンプ
	if (car->isOnGround()) {
		if (Pad(0).IsTrigger(enButtonA)) {
			static const int jumpParam = 5000;
			auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
			rigidbody->applyCentralImpulse(car->GetCarUp() * jumpParam);
			return car->GetCarState(Car::enInAir);
			//rigidbody->applyCentralImpulse(btVector3(0,1,0) * jumpParam);
		}
	}

	if (!car->isOnGround()) {
		return car->GetCarState(Car::enInAir);
	}

	return this;
}

void OnGroundState::Exit(Car* car) {

}


//空中状態
void InAirState::Enter(Car* car) {

}

CarState* InAirState::Update(Car* car) {
	//地面についたら地上ステートを返す
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//フリップ
	if (Pad(0).IsTrigger(enButtonA)) {
		m_isfripped = true;
		auto LStickX = Pad(0).GetLStickXF();
		auto LStickY = Pad(0).GetLStickYF();
		auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
		//rigidbody->applyCentralImpulse(m_upVec * 2000);
		//横フリップ
		auto Vright = car->GetCarRight();
		if (LStickX > 0) {
			rigidbody->applyCentralImpulse(Vright * 5000);
			//rigidbody->applyImpulse(m_upVec * 1000, -m_forwardVec);
		}
		else if (LStickX < 0) {
			rigidbody->applyCentralImpulse(-Vright * 5000);
		}
		//前後フリップ
		if (LStickY > 0) {
			rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * 4000);
			//rigidbody->applyImpulse(m_upVec * 8000, -m_forwardVec);
		}
		else if (LStickY < 0) {
			rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * -4000);
		}
		//最速ジャンプ
		//rigidbody->applyCentralImpulse(m_upVec * 2000);
	}
	if (m_isfripped) {
		m_cooltimer += IGameTime().GetFrameDeltaTime();
	}
	return this;
}

void InAirState::Exit(Car* car) {
	m_isfripped = false;
}