#include "stdafx.h"
#include "CarState.h"
#include "Car.h"

//�n����
void OnGroundState::Enter(Car* car) {

}

CarState* OnGroundState::Update(Car* car) {
	auto LStick = Pad(0).GetLStickXF();
	auto R2Trigger = Pad(0).GetRTrigger();
	auto L2Trigger = Pad(0).GetLTrigger();
	auto speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();

	float steeringClamp = 0.2f; //�X�e�A�����O����
	float defaultBreakingForce = 10.f;

	//�X�e�A�����O
	{
		const float defaultSteerringClamp = 0.3;
		float steering = LStick;
		float speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();
		//�l���������ݒ肷��قǍ����ŋȂ���ɂ����Ȃ�܂��B
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

	//�G���W���p���[
	{
		float engineForce = 0.f;
		//�O�i
		auto frontForce = R2Trigger * engineParam;
		if (car->GetRayCastVehicle()->getCurrentSpeedKmHour() > normalMaxSpeed)
			frontForce = 0;
		//printf("speed %f ... font force %f\n", speed,frontForce);
		//���
		auto backForce = L2Trigger * engineParam;

		car->SetEngineForce(frontForce - backForce);

		if (R2Trigger > 0.f or L2Trigger > 0.f) {
			car->SetBreakingForce(0.f);
		}
		else {
			car->SetBreakingForce(defaultBreakingForce);
		}
	}

	//�u�[�X�g
	if (Pad(0).IsPress(enButtonB)) {
		static const int boostParam = 20000;
		auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
		auto forward = car->GetRayCastVehicle()->getForwardVector();
		if (speed < boostMaxSpeed)
			rigidbody->applyCentralForce(forward * boostParam);
	}
	//�W�����v
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


//�󒆏��
void InAirState::Enter(Car* car) {

}

CarState* InAirState::Update(Car* car) {
	//�n�ʂɂ�����n��X�e�[�g��Ԃ�
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//�t���b�v
	if (Pad(0).IsTrigger(enButtonA)) {
		m_isfripped = true;
		auto LStickX = Pad(0).GetLStickXF();
		auto LStickY = Pad(0).GetLStickYF();
		auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
		//rigidbody->applyCentralImpulse(m_upVec * 2000);
		//���t���b�v
		auto Vright = car->GetCarRight();
		if (LStickX > 0) {
			rigidbody->applyCentralImpulse(Vright * 5000);
			//rigidbody->applyImpulse(m_upVec * 1000, -m_forwardVec);
		}
		else if (LStickX < 0) {
			rigidbody->applyCentralImpulse(-Vright * 5000);
		}
		//�O��t���b�v
		if (LStickY > 0) {
			rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * 4000);
			//rigidbody->applyImpulse(m_upVec * 8000, -m_forwardVec);
		}
		else if (LStickY < 0) {
			rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * -4000);
		}
		//�ő��W�����v
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