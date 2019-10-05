#include "stdafx.h"
#include "CarState.h"

////////////////////////////////////////////////////////////////////////////////////////////
//�n����
////////////////////////////////////////////////////////////////////////////////////////////

void OnGroundState::Enter(Car* car) {
	//printf("OnGround State\n");
}

CarState* OnGroundState::Update(Car* car) {
	//auto LStick = Pad(0).GetLStickXF();
	//auto R2Trigger = Pad(0).GetRTrigger();
	//auto L2Trigger = Pad(0).GetLTrigger();
	auto speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();

	float steeringClamp = 0.2f; //�X�e�A�����O����
	float defaultBreakingForce = 10.f;

	//�X�e�A�����O
	{
		const float defaultSteerringClamp = 0.3;
		float steering = m_inputs.steering;
		float speed = car->GetRayCastVehicle()->getCurrentSpeedKmHour();
		speed = abs(speed);
		//�l���������ݒ肷��قǍ����ŋȂ���ɂ����Ȃ�܂��B
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

	//�G���W���p���[
	{
		float engineForce = 0.f;
		//�O�i
		auto frontForce = m_inputs.accel * engineParam;
		if (car->GetRayCastVehicle()->getCurrentSpeedKmHour() > normalMaxSpeed)
			frontForce = 0;
		//printf("speed %f ... font force %f\n", speed,frontForce);
		//���
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

	//�W�����v
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
//�󒆏��
////////////////////////////////////////////////////////////////////////////////////////////
void InAirState::Enter(Car* car) {
	//printf("InAir State\n");
}

CarState* InAirState::Update(Car* car) {
	//�n�ʂɂ�����n��X�e�[�g��Ԃ�
	if (car->isOnGround()) return car->GetCarState(Car::enOnGround);

	//�t���b�v
	//if (Pad(0).IsTrigger(enButtonA)) 
	if(m_inputs.jump)
		return car->GetCarState(Car::enFlip);

	//Boost
	if(m_inputs.boost)
		Boost(car);

	//TODO : �����Ƃ���]���@�l����

	////�G�A���A��
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
		//�O����]
		totalRotVec.z = m_inputs.aerealX * -rotationSpeed;
		//auto rel = car->GetCarRight() * -50;
		//rigidbody->applyImpulse(car->GetCarUp() * rotationSpeed / 4 * m_inputs.aerealX, rel);
	}
	else {
		//�㎲��]
		totalRotVec.y = m_inputs.aerealX * rotationSpeed;
		//auto rel = car->GetCarForward() * 50;
		//rigidbody->applyImpulse(car->GetCarRight() * rotationSpeed * m_inputs.aerealX, rel);
	}
	//������]
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
//�t���b�v���
////////////////////////////////////////////////////////////////////////////////////////////
void FlipState::Enter(Car* car) {
	//printf("Flip State\n");
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

	//auto LStickX = Pad(0).GetLStickXF();
	//auto LStickY = Pad(0).GetLStickYF();
	auto rigidbody = car->GetRayCastVehicle()->getRigidBody();
	//rigidbody->applyCentralImpulse(m_upVec * 2000);
	//���t���b�v
	auto Vright = car->GetCarRight();
	if (m_inputs.aerealX > 0) {
		rigidbody->applyCentralImpulse(Vright * 5000);
		//rigidbody->applyImpulse(m_upVec * 1000, -m_forwardVec);
	}
	else if (m_inputs.aerealX < 0) {
		rigidbody->applyCentralImpulse(-Vright * 5000);
	}
	//�O��t���b�v
	if (m_inputs.aerealY > 0) {
		rigidbody->applyCentralImpulse(car->GetRayCastVehicle()->getForwardVector() * 4000);
		//rigidbody->applyImpulse(m_upVec * 8000, -m_forwardVec);
	}
	else if (m_inputs.aerealY < 0) {
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