#include "stdafx.h"
#include "CarState.h"
#include "Car.h"

////////////////////////////////////////////////////////////////////////////////////////////
//�n����
////////////////////////////////////////////////////////////////////////////////////////////

void OnGroundState::Enter(Car* car) {
	printf("OnGround State\n");
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
		static float clampParam = 8.f;
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

	//Boost
	Boost(car);
	
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



////////////////////////////////////////////////////////////////////////////////////////////
//�󒆏��
////////////////////////////////////////////////////////////////////////////////////////////
void InAirState::Enter(Car* car) {
	printf("InAir State\n");
}

CarState* InAirState::Update(Car* car) {
	//�n�ʂɂ�����n��X�e�[�g��Ԃ�
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//�t���b�v
	if (Pad(0).IsTrigger(enButtonA)) 
		return car->GetCarState(Car::enFlip);

	//Boost
	Boost(car);


	////�G�A���A��
	auto LStickX = Pad(0).GetLStickXF();
	auto LStickY = Pad(0).GetLStickYF();
	auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
	auto rdtr = rigidbody->getWorldTransform();
	auto rdpos = rdtr.getOrigin();
	auto rdrot = rdtr.getRotation();
	float rotationSpeed = 40.f;

	btVector3 totalAngularVelocity = rigidbody->getAngularVelocity();
	if (Pad(0).IsPress(enButtonRB1)) {
		//�O����]
		auto rel = car->GetCarRight() * -50;
		rigidbody->applyImpulse(car->GetCarUp() * rotationSpeed / 2 * LStickX, rel);
	}
	else {
		//�㎲��]
		auto rel = car->GetCarForward() * 50;
		rigidbody->applyImpulse(car->GetCarRight() * rotationSpeed * LStickX, rel);
	}
	//������]
	auto rel = car->GetCarForward() * -50;
	rigidbody->applyImpulse(car->GetCarUp() * rotationSpeed * LStickY, rel);

	return this;
}

void InAirState::Exit(Car* car) {

}


////////////////////////////////////////////////////////////////////////////////////////////
//�t���b�v���
////////////////////////////////////////////////////////////////////////////////////////////
void FlipState::Enter(Car* car) {
	printf("Flip State\n");
}

CarState* FlipState::Update(Car* car) {

	//�n�ʂɂ�����n��X�e�[�g��Ԃ�
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//��莞�Ԍo������󒆃X�e�[�g�ɖ߂�
	if (!m_isFirst) {
		m_cooltimer += IGameTime().GetFrameDeltaTime();
		if (m_cooltimer >= m_flipCoolTime) {
			return car->GetCarState(Car::enInAir);
		}
		return this;
	}

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
//�u�[�X�g����
////////////////////////////////////////////////////////////////////////////////////////////
static void Boost(Car* car) {
	if (Pad(0).IsPress(enButtonB)) {
		auto speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();
		const float boostMaxSpeed = 100.f;
		static const int boostParam = 20000;
		auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
		auto forward = car->GetRayCastVehicle()->getForwardVector();
		if (speed < boostMaxSpeed)
			rigidbody->applyCentralForce(forward * boostParam);
	}
}