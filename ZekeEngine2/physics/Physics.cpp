#include "stdafx.h"
#include "Physics.h"
#include "RigitBody.h"
#include "PhysicsDebugDraw.h"

#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

CPhysicsWorld::CPhysicsWorld() {

}

CPhysicsWorld::~CPhysicsWorld()
{
	Release();
}
void CPhysicsWorld::Release()
{
	delete dynamicWorld;
	delete constraintSolver;
	delete overlappingPairCache;
	delete collisionDispatcher;
	delete collisionConfig;
	delete debugDrawer;

	dynamicWorld = nullptr;
	constraintSolver = nullptr;
	overlappingPairCache = nullptr;
	collisionDispatcher = nullptr;
	collisionConfig = nullptr;
	debugDrawer = nullptr;
}
void CPhysicsWorld::Init()
{
	Release();
	//•¨—ƒGƒ“ƒWƒ“‚ð‰Šú‰»B
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfig = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	collisionDispatcher = new	btCollisionDispatcher(collisionConfig);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	constraintSolver = new btSequentialImpulseConstraintSolver;

	dynamicWorld = new btDiscreteDynamicsWorld(
		collisionDispatcher,
		overlappingPairCache,
		constraintSolver,
		collisionConfig
	);
	m_debugDraw = new PhysicsDebugDraw;
	m_debugDraw->Init();
	debugDrawer = m_debugDraw;
	dynamicWorld->setGravity(btVector3(0, -9.81, 0));
	dynamicWorld->setDebugDrawer(debugDrawer);
	debugDrawer->setDebugMode(1);
}

void CPhysicsWorld::Update()
{
	//dynamicWorld->stepSimulation(1.0f / 60.0f);
	int maxSimSubSteps = 2;

	int numSimSteps;
	numSimSteps = dynamicWorld->stepSimulation(IGameTime().GetFrameDeltaTime(), maxSimSubSteps);

	if (dynamicWorld->getConstraintSolver()->getSolverType() == BT_MLCP_SOLVER)
	{
		btMLCPSolver* sol = (btMLCPSolver*)dynamicWorld->getConstraintSolver();
		int numFallbacks = sol->getNumFallbacks();
		if (numFallbacks)
		{
			static int totalFailures = 0;
			totalFailures += numFallbacks;
		}
		sol->setNumFallbacks(0);
	}
}
void CPhysicsWorld::AddRigidBody(RigidBody& rb)
{
	dynamicWorld->addRigidBody(rb.GetBody());
}
void CPhysicsWorld::RemoveRigidBody(RigidBody& rb)
{
	dynamicWorld->removeRigidBody(rb.GetBody());
}

void CPhysicsWorld::DebugDraw() {
	m_debugDraw->BeginDraw();
	dynamicWorld->debugDrawWorld();
	m_debugDraw->EndDraw();
}