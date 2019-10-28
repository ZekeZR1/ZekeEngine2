#include "stdafx.h"
#include "CarState.h"

////////////////////////////////////////////////////////////////////////////////////////////
//地上状態
////////////////////////////////////////////////////////////////////////////////////////////

void OnGroundState::Enter(Car* car) {
	//printf("OnGround State\n");
}

CarState* OnGroundState::Update(Car* car) {
	//auto LStick = Pad(0).GetLStickXF();
	//auto R2Trigger = Pad(0).GetRTrigger();
	//auto L2Trigger = Pad(0).GetLTrigger();
	auto speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();

	float steeringClamp = 0.2f; //ステアリング制限
	float defaultBreakingForce = 10.f;

	//ステアリング
	{
		const float defaultSteerringClamp = 0.3;
		float steering = m_inputs.steering;
		float speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();
		speed = abs(speed);
		//値を小さく設定するほど高速で曲がりにくくなります。
		static float clampParam = 8.f;
		if (abs(speed) > 0) {
			steeringClamp = clampParam / abs(speed);
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
		auto frontForce = m_inputs.accel * engineParam;
		if (car->GetRayCastVehicle()->getCurrentSpeedKmHour() > normalMaxSpeed)
			frontForce = 0;
		//printf("speed %f ... font force %f\n", speed,frontForce);
		//後退
		auto backForce = m_inputs.back * engineParam;

		car->SetEngineForce(frontForce - backForce);


		//car->GetRayCastVehicle()->getRigidBody()->getTotalForce();


		if (m_inputs.accel > 0.f or m_inputs.back > 0.f) {
			car->SetBreakingForce(0.f);
		}
		else {
			car->SetBreakingForce(defaultBreakingForce);
		}
	}

	//Boost
	if(m_inputs.boost)
		Boost(car);

	//ジャンプ
	if (car->isOnGround()) {
		//if (Pad(0).IsTrigger(enButtonA)) {
		if (m_inputs.jump) {
			static const int jumpParam = 3000;
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



////////////////////////////////////////////////////////////////////////////////////////////
//空中状態
////////////////////////////////////////////////////////////////////////////////////////////
void InAirState::Enter(Car* car) {
	//printf("InAir State\n");
}

CarState* InAirState::Update(Car* car) {
	//地面についたら地上ステートを返す
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//フリップ
	//if (Pad(0).IsTrigger(enButtonA)) 
	if(m_inputs.jump)
		return car->GetCarState(Car::enFlip);

	//Boost
	if(m_inputs.boost)
		Boost(car);

	//TODO : 慣性とか回転方法考える

	////エアリアル
	//auto LStickX = Pad(0).GetLStickXF();
	//auto LStickY = Pad(0).GetLStickYF();
	auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
	auto rdtr = rigidbody->getWorldTransform();
	auto rdpos = rdtr.getOrigin();
	auto rdrot = rdtr.getRotation();
	float rotationSpeed = 2.5f;

	btVector3 totalAngularVelocity = rigidbody->getAngularVelocity();
	CVector3 totalRotVec = CVector3::Zero();
	//if (Pad(0).IsPress(enButtonRB1)) {
	if (m_inputs.airRoll) {
		//前軸回転
		totalRotVec.z = m_inputs.aerealX * -rotationSpeed;
		//auto rel = car->GetCarRight() * -50;
		//rigidbody->applyImpulse(car->GetCarUp() * rotationSpeed / 4 * m_inputs.aerealX, rel);
	}
	else {
		//上軸回転
		totalRotVec.y = m_inputs.aerealX * rotationSpeed;
		//auto rel = car->GetCarForward() * 50;
		//rigidbody->applyImpulse(car->GetCarRight() * rotationSpeed * m_inputs.aerealX, rel);
	}
	//横軸回転
	//CVector3 rotVec = CVector3(m_inputs.aerealY,0,0);
	totalRotVec.x += m_inputs.aerealY * rotationSpeed;
	auto wtr = car->GetRayCastVehicle()->getChassisWorldTransform();
	CQuaternion rot = wtr.getRotation();
	CMatrix mat;
	mat.MakeRotationFromQuaternion(rot);
	mat.Mul(totalRotVec);
	rigidbody->setAngularVelocity(totalAngularVelocity + totalRotVec);
	//auto rel = car->GetCarForward() * -50;
	//rigidbody->applyImpulse(car->GetCarUp() * rotationSpeed * 0.85 * m_inputs.aerealY, rel);

	return this;
}

void InAirState::Exit(Car* car) {

}


////////////////////////////////////////////////////////////////////////////////////////////
//フリップ状態
////////////////////////////////////////////////////////////////////////////////////////////
void FlipState::Enter(Car* car) {
	//printf("Flip State\n");
}

CarState* FlipState::Update(Car* car) {

	//地面についたら地上ステートを返す
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//一定時間経ったら空中ステートに戻す
	if (!m_isFirst) {
		m_cooltimer += IGameTime().GetFrameDeltaTime();
		if (m_cooltimer >= m_flipCoolTime) {
			return car->GetCarState(Car::enInAir);
		}
		return this;
	}

	//auto LStickX = Pad(0).GetLStickXF();
	//auto LStickY = Pad(0).GetLStickYF();
	auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
	//rigidbody->applyCentralImpulse(m_upVec * 2000);
	//横フリップ
	auto Vright = car->GetCarRight();
	if (m_inputs.aerealX > 0) {
		rigidbody->applyCentralImpulse(Vright * 5000);
		//rigidbody->applyImpulse(m_upVec * 1000, -m_forwardVec);
	}
	else if (m_inputs.aerealX < 0) {
		rigidbody->applyCentralImpulse(-Vright * 5000);
	}
	//前後フリップ
	if (m_inputs.aerealY > 0) {
		rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * 4000);
		//rigidbody->applyImpulse(m_upVec * 8000, -m_forwardVec);
	}
	else if (m_inputs.aerealY < 0) {
		rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * -4000);
	}
	//最速ジャンプ
	//else
		//rigidbody->applyCentralImpulse(car->GetCarUp() * 2000);
	m_isFirst = false;
	return this;
}

void FlipState::Exit(Car* car) {
	m_isFirst = false;
	m_cooltimer = 0.f;
}


////////////////////////////////////////////////////////////////////////////////////////////
//ブースト処理
////////////////////////////////////////////////////////////////////////////////////////////
static void Boost(Car* car) {
	//if (Pad(0).IsPress(enButtonB)) {
	{
		auto speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();
		const float boostMaxSpeed = 100.f;
		static const int boostParam = 10000;
		auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
		auto forward = car->GetRayCastVehicle()->getForwardVector();
		if (speed < boostMaxSpeed)
			rigidbody->applyCentralForce(forward * boostParam);
	}
}