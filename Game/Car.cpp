#include "stdafx.h"
#include "Car.h"

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
//#include "../CommonInterfaces/CommonExampleInterface.h"
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


btScalar maxMotorImpulse = 4000.f;

//the sequential impulse solver has difficulties dealing with large mass ratios (differences), between loadMass and the fork parts
btScalar loadMass = 350.f;  //
//btScalar loadMass = 10.f;//this should work fine for the SI solver

int rightIndex = 0;
int upIndex = 1;
int forwardIndex = 2;

static const int numWheel = 4;

btVector3 wheelDirectionCS0(0, -1, 0);
btVector3 wheelAxleCS(-1.f, 0.f, 0.f);

bool useMCLPSolver = true;


//
float gEngineForce = 0.f;

float defaultBreakingForce = 10.f;
float gBreakingForce = 100.f;

float maxEngineForce = 1000.f;  //this should be engine/velocity dependent
float maxBreakingForce = 100.f;

float gVehicleSteering = 0.f;
float steeringIncrement = 0.04f;
float steeringClamp = 0.2f;

float suspensionStiffness = 20.f;
float suspensionDamping = 2.3f;
float suspensionCompression = 4.4f;
float rollInfluence = 0.1f;  //1.0f;

btScalar suspensionRestLength(0.6);

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

	delete m_indexVertexArrays;
	delete m_vertices;

	delete m_vehicleRayCaster;
	m_vehicleRayCaster = 0;

	delete m_vehicle;
	m_vehicle = 0;

	delete m_wheelShape;
	m_wheelShape = 0;

	//delete solver
	delete m_constraintSolver;
	m_constraintSolver = 0;

	//delete broadphase
	delete m_overlappingPairCache;
	m_overlappingPairCache = 0;

	//delete dispatcher
	delete m_dispatcher;
	m_dispatcher = 0;

	delete m_collisionConfiguration;
	m_collisionConfiguration = 0;
}

void Car::init() {
	int upAxis = 1;

	modelInit();

	{
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		btVector3 worldMin(-1000, -1000, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		m_overlappingPairCache = new btAxisSweep3(worldMin, worldMax);
		if (useMCLPSolver)
		{
			btDantzigSolver* mlcp = new btDantzigSolver();
			//btSolveProjectedGaussSeidel* mlcp = new btSolveProjectedGaussSeidel;
			btMLCPSolver* sol = new btMLCPSolver(mlcp);
			m_constraintSolver = sol;
		}
		else
		{
			m_constraintSolver = new btSequentialImpulseConstraintSolver();
		}
		if (useMCLPSolver)
		{
			m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 1;  //for direct solver it is better to have a small A matrix
		}
		else
		{
			m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 128;  //for direct solver, it is better to solve multiple objects together, small batches have high overhead
		}
		m_dynamicsWorld->getSolverInfo().m_globalCfm = 0.00001;
	}

	//m_dynamicsWorld->setGravity(btVector3(0,0,0));
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
	localTrans.setOrigin(btVector3(0, 1, 0));

	compound->addChildShape(localTrans, chassisShape);

	{
		//btCollisionShape* suppShape = new btBoxShape(btVector3(0.5f, 0.1f, 0.5f));
		//btTransform suppLocalTrans;
		//suppLocalTrans.setIdentity();
		////localTrans effectively shifts the center of mass with respect to the chassis
		//suppLocalTrans.setOrigin(btVector3(0, 1.0, 2.5));
		//compound->addChildShape(suppLocalTrans, suppShape);
	}

	tr.setOrigin(btVector3(0, 0.f, 0));

	m_carChassis = localCreateRigidBody(800, tr, compound);  //chassisShape);

	m_carChassis->setRestitution(0.5f);

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
		m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);

		///never deactivate the vehicle
		m_carChassis->setActivationState(DISABLE_DEACTIVATION);

		m_dynamicsWorld->addVehicle(m_vehicle);

		float connectionHeight = 1.2f;

		bool isFrontWheel = true;

		//choose coordinate system
		m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

		//static const float wheelposz
		btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3 * wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius);

		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3 * wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius);

		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3 * wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius);
		isFrontWheel = false;
		m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
		connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3 * wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius);
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

	ResetCar();
}

void Car::stepSimulation() {
	{
		int wheelIndex = 2;
		m_vehicle->applyEngineForce(gEngineForce, wheelIndex);
		m_vehicle->setBrake(gBreakingForce, wheelIndex);
		wheelIndex = 3;
		m_vehicle->applyEngineForce(gEngineForce, wheelIndex);
		m_vehicle->setBrake(gBreakingForce, wheelIndex);
		wheelIndex = 0;
		m_vehicle->setSteeringValue(gVehicleSteering, wheelIndex);
		wheelIndex = 1;
		m_vehicle->setSteeringValue(gVehicleSteering, wheelIndex);

	}

	modelUpdate();
	
	float dt = IGameTime().GetFrameDeltaTime();

	if (m_dynamicsWorld)
	{
		//during idle mode, just run 1 simulation step maximum
		int maxSimSubSteps = 2;

		int numSimSteps;
		numSimSteps = m_dynamicsWorld->stepSimulation(dt, maxSimSubSteps);

		if (m_dynamicsWorld->getConstraintSolver()->getSolverType() == BT_MLCP_SOLVER)
		{
			btMLCPSolver* sol = (btMLCPSolver*)m_dynamicsWorld->getConstraintSolver();
			int numFallbacks = sol->getNumFallbacks();
			if (numFallbacks)
			{
				static int totalFailures = 0;
				totalFailures += numFallbacks;
			}
			sol->setNumFallbacks(0);
		}
	}
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
				m_frontLeftWheel->SetRotation(rfixed);
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
				CQuaternion rfixed = CQuaternion::Identity();
				rfixed.SetRotationDeg(CVector3::AxisY(), 180.f);
				rfixed.Multiply(rot);
				m_rearRightWheel->SetPosition(origin);
				m_rearRightWheel->SetRotation(rfixed);
			}
		}
	}
}

void  Car::buttonUpdate() {

	auto LStick = Pad(0).GetLStickXF();
	auto R2Trigger = Pad(0).GetRTrigger();
	auto L2Trigger = Pad(0).GetLTrigger();

	//ステアリング
	{
		const float defaultSteerringClamp = 0.3;
		gVehicleSteering = LStick;
		float speed = m_vehicle->getCurrentSpeedKmHour();
		auto sr = gVehicleSteering;
		static float clampParam = 9.5f;
		if (speed > 0) {
			steeringClamp = clampParam / speed;
		}
		if (steeringClamp > defaultSteerringClamp)
			steeringClamp = defaultSteerringClamp;
		if (gVehicleSteering > steeringClamp)
			gVehicleSteering = steeringClamp;
		if (gVehicleSteering < -steeringClamp)
			gVehicleSteering = -steeringClamp;

	}

	//エンジンパワー
	{
		static const int engineParam = 1000;
		float engineForce = 0.f;
		//前進
		auto frontForce = R2Trigger * engineParam;
		//後退
		auto backForce = L2Trigger * engineParam;

		gEngineForce = frontForce - backForce;
		
		if (R2Trigger > 0.f or L2Trigger > 0.f) {
			gBreakingForce = 0.f;
		}
		else {
			gBreakingForce = defaultBreakingForce;
		}
	}
	
	//ブースト
	if (Pad(0).IsPress(enButtonB)) {
		static const int boostParam = 20000;
		auto rigidbody = m_vehicle->getRigidBody();
		auto forward = m_vehicle->getForwardVector();
		rigidbody->applyCentralForce(forward * boostParam);
	}
	//ジャンプ
	if (Pad(0).IsTrigger(enButtonA)) {
		static const int jumpParam = 5000;
		auto rigidbody = m_vehicle->getRigidBody();
		auto forward = m_vehicle->getForwardVector();
		//TODO : 車の上方向に力を加える
		rigidbody->applyCentralImpulse(btVector3(0,1,0) * jumpParam);
	}
	//エアリアル制御
	Aerial();
	//リセット
	if (Pad(0).IsTrigger(enButtonStart)) {
		ResetCar();
	}
}

 void Car::ResetCar() {
	 gVehicleSteering = 0.f;
	 gBreakingForce = defaultBreakingForce;
	 gEngineForce = 0.f;

	 m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	 m_carChassis->setLinearVelocity(btVector3(0, 0, 0));
	 m_carChassis->setAngularVelocity(btVector3(0, 0, 0));
	 m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), getDynamicsWorld()->getDispatcher());
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

		//前輪ホイール
		m_frontLeftWheel = NewGO<SkinModelRender>(0);
		m_frontLeftWheel->Init(L"Assets/modelData/frontWheel.cmo");
		m_frontLeftWheel->SetScale(m_wheelModelScale);
		m_frontRightWheel = NewGO<SkinModelRender>(0);
		m_frontRightWheel->Init(L"Assets/modelData/frontWheel.cmo");
		m_frontRightWheel->SetScale(m_wheelModelScale);

		//後輪ホイール
		m_rearRightWheel = NewGO<SkinModelRender>(0);
		m_rearRightWheel->Init(L"Assets/modelData/rearWheel.cmo");
		m_rearRightWheel->SetScale(m_wheelModelScale);
		m_rearLeftWheel = NewGO<SkinModelRender>(0);
		m_rearLeftWheel->Init(L"Assets/modelData/rearWheel.cmo");
		m_rearLeftWheel->SetScale(m_wheelModelScale);
	}
}

void Car::Aerial() {
	//エアリアル
	//auto force = m_vehicle->getRigidBody()->getTotalForce();
	auto wtr = m_vehicle->getWheelTransformWS(0);
	//TODO : とりあえず床走ってるときしか考えてない
	if (wtr.getOrigin().getY() < 0.6f) return;

	{
		auto LStickX = Pad(0).GetLStickXF();
		auto LStickY = Pad(0).GetLStickYF();
		auto rigidbody = m_vehicle->getRigidBody();
		auto rdtr = rigidbody->getWorldTransform();
		auto rdrot = rdtr.getRotation();
		float rotationSpeed = 3.f;
		auto forward = m_vehicle->getForwardVector();
		//printf("forward x : %f y : %f z : %f", forward.getX(), forward.getY(), forward.getZ());
		if (Pad(0).IsPress(enButtonRB1)) {
			//前軸回転
			CQuaternion rot = CQuaternion::Identity();
			rot.SetRotationDeg({ 0,0,1 }, LStickX * -rotationSpeed);
			rdrot *= btQuaternion(rot);
		}
		else {
			//上軸回転
			CQuaternion rot = CQuaternion::Identity();
			rot.SetRotationDeg({ 0,1,0 }, LStickX * rotationSpeed);
			rdrot *= btQuaternion(rot);
		}

		//横軸回転
		CQuaternion rot = CQuaternion::Identity();
		rot.SetRotationDeg({ 1,0,0 }, LStickY * rotationSpeed);
		rdrot *= btQuaternion(rot);

		rdtr.setRotation(rdrot);
		rigidbody->setWorldTransform(rdtr);
	}
}