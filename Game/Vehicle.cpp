#include "stdafx.h"
#include "Vehicle.h"

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

class btVehicleTuning;
class btCollisionShape;

#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "btBulletCollisionCommon.h"



Vehicle::Vehicle() {
	m_dynamicsWorld = PhysicsWorld().GetDynamicWorld();
}

Vehicle::~Vehicle() {
	RemoveRigidBoddy();
}

void Vehicle::init() {

	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0, -3, 0));

	//シャーシー
	btCollisionShape* chassisShape = new btBoxShape(btVector3(1.f, 0.5f, 2.f));
	m_collisionShapes.push_back(chassisShape);
	auto chassisModel = NewGO<SkinModelRender>(0);
	chassisModel->Init(L"Assets/modelData/chassis.cmo");
	m_carModels.push_back(chassisModel);

	//形状を組み合わせる
	btCompoundShape* compound = new btCompoundShape();
	m_collisionShapes.push_back(compound);
	btTransform localTrans;
	localTrans.setIdentity();
	localTrans.setOrigin(btVector3(0, 1, 0));
	compound->addChildShape(localTrans, chassisShape);

	{
		btCollisionShape* suppShape = new btBoxShape(btVector3(0.5f, 0.1f, 0.5f));
		btTransform suppLocalTrans;
		suppLocalTrans.setIdentity();
		//localTrans effectively shifts the center of mass with respect to the chassis
		suppLocalTrans.setOrigin(btVector3(0, 1.0, 2.5));
		compound->addChildShape(suppLocalTrans, suppShape);
	}

	const btScalar FALLHEIGHT = 5;
	tr.setOrigin(btVector3(0, FALLHEIGHT, 0));

	const btScalar chassisMass = 2.0f;
	const btScalar wheelMass = 1.0f;
	m_carChassis = localCreateRigidBody(chassisMass, tr, compound);

	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

	btVector3 wheelPos[4] = {
		btVector3(btScalar(-1.), btScalar(FALLHEIGHT - 0.25), btScalar(1.25)),
		btVector3(btScalar(1.), btScalar(FALLHEIGHT - 0.25), btScalar(1.25)),
		btVector3(btScalar(1.), btScalar(FALLHEIGHT - 0.25), btScalar(-1.25)),
		btVector3(btScalar(-1.), btScalar(FALLHEIGHT - 0.25), btScalar(-1.25)) 
	};

	for (int i = 0; i < 4; i++)
	{
		// create a Hinge2 joint
		// create two rigid bodies
		// static bodyA (parent) on top:

		btRigidBody* pBodyA = this->m_carChassis;
		pBodyA->setActivationState(DISABLE_DEACTIVATION);
		// dynamic bodyB (child) below it :
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(wheelPos[i]);

		{
			auto wheel = NewGO<SkinModelRender>(0);
			wheel->Init(L"Assets/modelData/wheel.cmo");
			wheel->SetPosition({ wheelPos[i].getX(),wheelPos[i].getY(),wheelPos[i].getZ() });
			m_carModels.push_back(wheel);
		}

		btRigidBody* pBodyB = createRigidBody(wheelMass, tr, m_wheelShape);
		pBodyB->setFriction(1110);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);
		// add some data to build constraint frames
		btVector3 parentAxis(0.f, 1.f, 0.f);
		btVector3 childAxis(1.f, 0.f, 0.f);
		btVector3 anchor = tr.getOrigin();
		btHinge2Constraint* pHinge2 = new btHinge2Constraint(*pBodyA, *pBodyB, anchor, parentAxis, childAxis);


		// add constraint to world
		auto dw = PhysicsWorld().GetDynamicWorld();
		dw->addConstraint(pHinge2, true);

		// Drive engine.
		pHinge2->enableMotor(3, true);
		pHinge2->setMaxMotorForce(3, 1000);
		pHinge2->setTargetVelocity(3, 0);

		// Steering engine.
		pHinge2->enableMotor(5, true);
		pHinge2->setMaxMotorForce(5, 1000);
		pHinge2->setTargetVelocity(5, 0);

		//CFM 0 is hard
		pHinge2->setParam(BT_CONSTRAINT_CFM, 0.13f, 2);
		//0から1の値を設定
		pHinge2->setParam(BT_CONSTRAINT_ERP, 0.3, 2);

		pHinge2->setDamping(2, 2.0);
		pHinge2->setStiffness(2, 350.0);

		pHinge2->setDbgDrawSize(btScalar(5.f));

	}
}

void Vehicle::stepSimulation() {
	float dt = IGameTime().GetFrameDeltaTime();

	if (m_dynamicsWorld)
	{
		//during idle mode, just run 1 simulation step maximum
		int maxSimSubSteps = 2;

		int numSimSteps;
		//numSimSteps = m_dynamicsWorld->stepSimulation(dt, maxSimSubSteps);

		if (m_dynamicsWorld->getConstraintSolver()->getSolverType() == BT_MLCP_SOLVER)
		{
			btMLCPSolver* sol = (btMLCPSolver*)m_dynamicsWorld->getConstraintSolver();
			int numFallbacks = sol->getNumFallbacks();
			if (numFallbacks)
			{
				static int totalFailures = 0;
				totalFailures += numFallbacks;
				printf("MLCP solver failed %d times, falling back to btSequentialImpulseSolver (SI)\n", totalFailures);
			}
			sol->setNumFallbacks(0);
		}

		//#define VERBOSE_FEEDBACK
#ifdef VERBOSE_FEEDBACK
		if (!numSimSteps)
			printf("Interpolated transforms\n");
		else
		{
			if (numSimSteps > maxSimSubSteps)
			{
				//detect dropping frames
				printf("Dropped (%i) simulation steps out of %i\n", numSimSteps - maxSimSubSteps, numSimSteps);
			}
			else
			{
				printf("Simulated (%i) steps\n", numSimSteps);
			}
		}
#endif  //VERBOSE_FEEDBACK
	}
	//m_dynamicsWorld->debugDrawWorld();
}


btRigidBody* Vehicle::localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
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
	auto dw = PhysicsWorld().GetDynamicWorld();
	dw->addRigidBody(body);
	//m_dynamicsWorld->addRigidBody(body);
	return body;
}


btRigidBody* Vehicle::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color)
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

	body->setUserIndex(-1);
	auto dw = PhysicsWorld().GetDynamicWorld();
	dw->addRigidBody(body);
	return body;
}


void Vehicle::RemoveRigidBoddy() {
	//PhysicsWorld().RemoveRigidBody(m_rigidBody);

	//cleanup in the reverse order of creation/initialization

//remove the rigidbodies from the dynamics world and delete them
	//int i;
	//for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	//{
	//	btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	if (body && body->getMotionState())
	//	{
	//		while (body->getNumConstraintRefs())
	//		{
	//			btTypedConstraint* constraint = body->getConstraintRef(0);
	//			m_dynamicsWorld->removeConstraint(constraint);
	//			delete constraint;
	//		}
	//		delete body->getMotionState();
	//		m_dynamicsWorld->removeRigidBody(body);
	//	}
	//	else
	//	{
	//		m_dynamicsWorld->removeCollisionObject(obj);
	//	}
	//	delete obj;
	//}

	//delete collision shapes
	for (int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	//delete m_indexVertexArrays;
	//delete m_vertices;

	delete m_wheelShape;
	m_wheelShape = 0;

	//delete solver
	//delete m_solver;
	//m_solver = 0;

	//delete broadphase
	//delete m_broadphase;
	//m_broadphase = 0;

//	//delete dispatcher
//	delete m_dispatcher;
//	m_dispatcher = 0;
//
//	delete m_collisionConfiguration;
//	m_collisionConfiguration = 0;
}

void Vehicle::KeyUpdate() {
	if (Pad(0).IsPress(enButtonLeft)) {
		gVehicleSteering += steeringIncrement;
		if (gVehicleSteering > steeringClamp)
			gVehicleSteering = steeringClamp;
	}
	if (Pad(0).IsPress(enButtonRight)) {
		gVehicleSteering -= steeringIncrement;
		if (gVehicleSteering < -steeringClamp)
			gVehicleSteering = -steeringClamp;
	}
	if (Pad(0).IsPress(enButtonUp)) {
		gEngineForce = maxEngineForce;
		gBreakingForce = 0.f;
	}
	if (Pad(0).IsPress(enButtonDown)) {
		gEngineForce = -maxEngineForce;
		gBreakingForce = 0.f;
	}
		//case B3G_F7:
		//{
		///*	handled = true;
		//	btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
		//	world->setLatencyMotionStateInterpolation(!world->getLatencyMotionStateInterpolation());
		//	printf("world latencyMotionStateInterpolation = %d\n", world->getLatencyMotionStateInterpolation());*/
		//	break;
		//}
		//case B3G_F6:
		//{
		//	handled = true;
		//	//switch solver (needs demo restart)
		//	useMCLPSolver = !useMCLPSolver;
		//	printf("switching to useMLCPSolver = %d\n", useMCLPSolver);

		//	delete m_solver;
		//	if (useMCLPSolver)
		//	{
		//		btDantzigSolver* mlcp = new btDantzigSolver();
		//		//btSolveProjectedGaussSeidel* mlcp = new btSolveProjectedGaussSeidel;
		//		btMLCPSolver* sol = new btMLCPSolver(mlcp);
		//		m_solver = sol;
		//	}
		//	else
		//	{
		//		m_solver = new btSequentialImpulseConstraintSolver();
		//	}
		//	m_dynamicsWorld->setConstraintSolver(m_solver);
		//	//exitPhysics();
		//	//initPhysics();
		//	break;
		//}
}