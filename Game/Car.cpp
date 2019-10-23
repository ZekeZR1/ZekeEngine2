#include "stdafx.h"
#include "Car.h"
#include "CarState.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

class btVehicleTuning;
struct btVehicleRaycaster;
class btCollisionShape;

#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "btBulletCollisionCommon.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

//btScalar maxMotorImpulse = 4000.f;

//the sequential impulse solver has difficulties dealing with large mass ratios (differences), between loadMass and the fork parts
//btScalar loadMass = 350.f;  //
//btScalar loadMass = 10.f;//this should work fine for the SI solver

int rightIndex = 0;
int upIndex = 1;
int forwardIndex = 2;

static const int numWheel = 4;

btVector3 wheelDirectionCS0(0, -1, 0);
btVector3 wheelAxleCS(-1.f, 0.f, 0.f);

bool useMCLPSolver = true;



float maxEngineForce = 1000.f;  //this should be engine/velocity dependent
float maxBreakingForce = 100.f;

float steeringIncrement = 0.04f;

float suspensionStiffness = 10.f; 
float suspensionDamping = 2.13f; 
float suspensionCompression = 14.4f; 
float rollInfluence = 0.0f; 

//btScalar suspensionRestLength(0.7);
btScalar suspensionRestLength(0.55);

#define CUBE_HALF_EXTENTS 1


Car::Car() {
	m_dynamicsWorld = PhysicsWorld().GetDynamicWorld();
	init();
}

Car::~Car() {

	//delete collision shapes
	for (int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}

	m_collisionShapes.clear();

	m_dynamicsWorld->removeVehicle(m_vehicle);

	m_dynamicsWorld->removeRigidBody(m_carChassis);

	delete m_vehicleRayCaster;
	m_vehicleRayCaster = 0;

	delete m_vehicle;
	m_vehicle = 0;

	delete m_wheelShape;
	m_wheelShape = 0;

	for (auto s : m_StatePool)
		delete s;
}

bool Car::Start() {
	return true;
}

void Car::OnDestroy() {
	for (auto model : m_models) {
		DeleteGO(model);
	}
}

void Car::Update() {
	stepSimulation();
	buttonUpdate();
	MoveLimit();
}

void Car::init() {
	int upAxis = 1;
	//状態クラスを生成しておく
	m_StatePool.push_back(new OnGroundState);
	m_StatePool.push_back(new InAirState);
	m_StatePool.push_back(new FlipState);
	//地上状態からスタート
	m_state = GetCarState(enOnGround);

	modelInit();

	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0, -3, 0));

	//either use heightfield or triangle mesh

	//シャーシー
	btCollisionShape* chassisShape = new btBoxShape(btVector3(m_chassisShapeSize.x, m_chassisShapeSize.y, m_chassisShapeSize.z));
	m_collisionShapes.push_back(chassisShape);

	btCompoundShape* compound = new btCompoundShape();
	m_collisionShapes.push_back(compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0, 0, 0));

	compound->addChildShape(localTrans, chassisShape);

	{
		//btCollisionShape* suppShape = new btBoxShape(btVector3(0.5f, 0.1f, 0.5f));
		//btTransform suppLocalTrans;
		//suppLocalTrans.setIdentity();
		////localTrans effectively shifts the center of mass with respect to the chassis
		//suppLocalTrans.setOrigin(btVector3(0, 1.0, 2.5));
		//compound->addChildShape(suppLocalTrans, suppShape);
	}

	//シャーシーの位置を中心にする
	tr.setOrigin(btVector3(0, 0.f, 0));

	//m_carChassis = localCreateRigidBody(800, tr, compound);  //chassisShape);
	m_carChassis = localCreateRigidBody(m_chassisMass, tr, compound);  //chassisShape);

	//m_carChassis->setRestitution(0.5f);

	//m_carChassis->setDamping(0.2,0.2);

	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

	int wheelGraphicsIndex = m_wheelShape->getUserIndex();

	const float position[4] = { 0, 10, 10, 0 };
	const float quaternion[4] = { 0, 0, 0, 1 };
	const float color[4] = { 0, 1, 0, 1 };
	const float scaling[4] = { 1, 1, 1, 1 };

	for (int i = 0; i < 4; i++)
	{
		//m_wheelInstances[i] = m_guiHelper->registerGraphicsInstance(wheelGraphicsIndex, position, quaternion, color, scaling);
	}

	/// create vehicle
	{
		m_vehicleRayCaster = new btDefaultVehicleRaycaster(m_dynamicsWorld);
		//m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);
		m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);
		//m_vehicle = new MyVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);

		///never deactivate the vehicle
		m_carChassis->setActivationState(DISABLE_DEACTIVATION);

		m_dynamicsWorld->addVehicle(m_vehicle);
		//choose coordinate system
		m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

		//ホイール作成登録
		bool isFrontWheel = true;
		static const float WheelXDistanceFix = 0.2f;
		//前輪
		static const float frontWheelZfix = -0.2f;
		btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3 * wheelWidth) - WheelXDistanceFix, connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius + frontWheelZfix);
		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3 * wheelWidth) + WheelXDistanceFix, connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius + frontWheelZfix);
		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
		//後輪
		isFrontWheel = false;
		static const float rearWheelZfix = -0.6f;
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3 * wheelWidth) + WheelXDistanceFix, connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius - rearWheelZfix);
		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
		connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3 * wheelWidth) - WheelXDistanceFix, connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius - rearWheelZfix);
		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

		for (int i = 0; i < m_vehicle->getNumWheels(); i++)
		{
			btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = suspensionDamping;
			wheel.m_wheelsDampingCompression = suspensionCompression;
			wheel.m_frictionSlip = wheelFriction;
			wheel.m_rollInfluence = rollInfluence;
		}
	}
	//m_vehicle->getRigidBody()->setGravity(btVector3(0, -10, 0));
	ResetCar();
}

void Car::stepSimulation() {
	//現在の状態を更新
	auto nextState = m_state->Update(this);
	//状態が変化したら状態初期化
	if (nextState != m_state) {
		m_state->Exit(this);
		nextState->Enter(this);
		m_state = nextState;
	}

	{
		int wheelIndex = 2;
		m_vehicle->applyEngineForce(m_engineForce, wheelIndex);
		m_vehicle->setBrake(m_breakingForce, wheelIndex);
		wheelIndex = 3;
		m_vehicle->applyEngineForce(m_engineForce, wheelIndex);
		m_vehicle->setBrake(m_breakingForce, wheelIndex);
		wheelIndex = 0;
		m_vehicle->setSteeringValue(m_vehicleSteering, wheelIndex);
		wheelIndex = 1;
		m_vehicle->setSteeringValue(m_vehicleSteering, wheelIndex);
	}

	if (m_vehicle->numWheelsOnGround < 4) {
		if (m_isHostPlayerCar) {
			static btVector3 nav(0, 0, 0);
			m_carChassis->setAngularVelocity(nav);
		}
	}

	//TODO : プログラムで判定せずにポリゴンからデータをとってきたい・・・
	{
		if (GetRayCastVehicle()->numWheelsOnGround > 0) {
			static const float attr = 6000.f;
			auto force = GetCarUp();
			force *= attr;
			m_vehicle->getRigidBody()->applyCentralForce(-force);
		}
	}

	//地上に居そうなときは車が地面と平行になるような力を加える
	{

		auto numWheelOnGround = GetRayCastVehicle()->numWheelsOnGround;
		auto chassisY = GetRayCastVehicle()->getChassisWorldTransform().getOrigin().getY();
		//printf("car y Pos ..%f\n", chassisY);
		if (numWheelOnGround > 0 and numWheelOnGround != 4 and chassisY < 3.0f) {
			static const float y = 0.01;
			//右に傾いている
			if (m_rightVec.getY() < -y) {
				auto rel = GetCarRight() * -50;
				m_vehicle->getRigidBody()->applyImpulse(GetCarUp() * -10, rel);
			}
			//左に傾いている
			if (m_rightVec.getY() > y) {
				auto rel = GetCarRight() * -50;
				m_vehicle->getRigidBody()->applyImpulse(GetCarUp() * 10, rel);
			}
			//下に傾いてる
			if (m_forwardVec.getY() < -y) {
				auto rel = GetCarForward() * -50;
				m_vehicle->getRigidBody()->applyImpulse(GetCarUp() * -20, rel);
			}
			//上に傾いてる
			if (m_forwardVec.getY() > y) {
				auto rel = GetCarForward() * -50;
				m_vehicle->getRigidBody()->applyImpulse(GetCarUp() * 20, rel);
			}
		}
	}

	if (m_vehicle->numWheelsOnGround == 4) {
		m_isOnGround = true;
	}
	else {
		m_isOnGround = false;
	}
	modelUpdate();


	 m_forwardVec = m_vehicle->getForwardVector();

	const btTransform& chassisTrans = m_vehicle->getChassisWorldTransform();
	m_rightVec = btVector3(
		chassisTrans.getBasis()[0][0],
		chassisTrans.getBasis()[1][0],
		chassisTrans.getBasis()[2][0]);
	m_rightVec.normalize();

	m_upVec = btVector3(
		chassisTrans.getBasis()[0][1],
		chassisTrans.getBasis()[1][1],
		chassisTrans.getBasis()[2][1]);
	m_upVec.normalize();
}

void Car::modelUpdate() {
	{
		//シャーシーのワールド行列ををモデルにセット
		{
			auto chassisTransform = m_vehicle->getChassisWorldTransform();
			auto origin = chassisTransform.getOrigin();
			auto chassisRotation = chassisTransform.getRotation();
			CVector3 chassisPosition = origin;
			chassisPosition += m_chassisPositionFix;
			m_chassiModel->SetPosition(chassisPosition);
			m_chassiModel->SetRotation(chassisRotation);
		}
		//ホイールのワールド行列も同じ

		{
			{
				//左前輪
				auto wheelTransform = m_vehicle->getWheelTransformWS(1);
				auto origin = wheelTransform.getOrigin();
				auto rot = wheelTransform.getRotation();
				CQuaternion rfixed = CQuaternion::Identity();
				rfixed.SetRotationDeg(CVector3::AxisY(), 180.f);
				rfixed.Multiply(rot);
				m_frontLeftWheel->SetPosition(origin);
				//m_frontLeftWheel->SetRotation(rfixed);
				m_frontLeftWheel->SetRotation(rot);
			}
			{
				//右前輪
				auto wheelTransform = m_vehicle->getWheelTransformWS(0);
				auto origin = wheelTransform.getOrigin();
				auto rot = wheelTransform.getRotation();
				m_frontRightWheel->SetPosition(origin);
				m_frontRightWheel->SetRotation(rot);
			}
			{
				//左後輪
				auto wheelTransform = m_vehicle->getWheelTransformWS(2);
				auto origin = wheelTransform.getOrigin();
				auto rot = wheelTransform.getRotation();
				m_rearLeftWheel->SetPosition(origin);
				m_rearLeftWheel->SetRotation(rot);
			}

			{
				//右後輪
				auto wheelTransform = m_vehicle->getWheelTransformWS(3);
				auto origin = wheelTransform.getOrigin();
				auto rot = wheelTransform.getRotation();
				m_rearRightWheel->SetPosition(origin);
				m_rearRightWheel->SetRotation(rot);
			}
		}
	}
}

void  Car::buttonUpdate() {

	m_state->SetInput(m_carInputs);
	m_state->Update(this);
}

 void Car::ResetCar(CVector3 resetPos, CQuaternion resetQuaternion) {
	 m_vehicleSteering = 0.f;
	 m_engineForce = 0.f;
	 m_breakingForce = 100.f;
	 //ちょっと上に生成
	 auto wtr = m_vehicle->getRigidBody()->getWorldTransform();
	 //wtr.setOrigin(btVector3(0, 2, -10));
	 wtr.setOrigin(resetPos);
	 //wtr.setRotation(btQuaternion::getIdentity());
	 wtr.setRotation(resetQuaternion);
	 m_carChassis->setCenterOfMassTransform(wtr);
	 m_carChassis->setLinearVelocity(btVector3(0, 0, 0));
	 m_carChassis->setAngularVelocity(btVector3(0, 0, 0));
	 m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), m_dynamicsWorld->getDispatcher());
	 if (m_vehicle)
	 {
		 m_vehicle->resetSuspension();
		 for (int i = 0; i < m_vehicle->getNumWheels(); i++)
		 {
			 //synchronize the wheels with the (interpolated) chassis worldtransform
			 m_vehicle->updateWheelTransform(i, true);
		 }
	 }
 }

btRigidBody* Car::localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState * myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
	btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif  //

	m_dynamicsWorld->addRigidBody(body);
	return body;
}


void Car::modelInit() {
	{
		//シャーシー
		m_chassiModel = NewGO<SkinModelRender>(0);
		m_chassiModel->Init(L"Assets/modelData/chassis.cmo");
		m_chassiModel->SetScale(m_chassisModelScale);
		m_models.push_back(m_chassiModel);
		//前輪ホイール
		m_frontLeftWheel = NewGO<SkinModelRender>(0);
		m_models.push_back(m_frontLeftWheel);
		m_frontLeftWheel->Init(L"Assets/modelData/frontLeftWheel.cmo");
		m_frontLeftWheel->SetScale(m_wheelModelScale);
		m_frontRightWheel = NewGO<SkinModelRender>(0);
		m_models.push_back(m_frontRightWheel);
		m_frontRightWheel->Init(L"Assets/modelData/frontRightWheel.cmo");
		m_frontRightWheel->SetScale(m_wheelModelScale);
		//後輪ホイール
		m_rearRightWheel = NewGO<SkinModelRender>(0);
		m_models.push_back(m_rearRightWheel);
		m_rearRightWheel->Init(L"Assets/modelData/rearRightWheel.cmo");
		m_rearRightWheel->SetScale(m_wheelModelScale);
		m_rearLeftWheel = NewGO<SkinModelRender>(0);
		m_models.push_back(m_rearLeftWheel);
		m_rearLeftWheel->Init(L"Assets/modelData/rearLeftWheel.cmo");
		m_rearLeftWheel->SetScale(m_wheelModelScale);
	}
}



void Car::MoveLimit() {
	if (GetPosition().y < 0.f) {
		auto wtr = m_vehicle->getRigidBody()->getWorldTransform();
		CVector3 np;
		np.x = wtr.getOrigin().getX();
		np.y = 0.3f;
		np.z = wtr.getOrigin().getZ();
		wtr.setOrigin(np);
		m_carChassis->setCenterOfMassTransform(wtr);
	}
}

void Car::SetCarInput(CarControll& input) {
	m_carInputs.accel = input.accel;
	m_carInputs.back = input.back;
	m_carInputs.steering = input.steering;
	m_carInputs.aerealX = input.aerealX;
	m_carInputs.aerealY = input.aerealY;
	m_carInputs.jump = input.jump;
	m_carInputs.boost = input.boost;
	m_carInputs.airRoll = input.airRoll;
}

void Car::SetTransform(CVector3 pos, CQuaternion rot) {
	auto wtr = m_vehicle->getRigidBody()->getWorldTransform();
	wtr.setOrigin(pos);
	wtr.setRotation(rot);
	m_carChassis->setCenterOfMassTransform(wtr);
	m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), m_dynamicsWorld->getDispatcher());
}