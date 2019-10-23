#pragma once
#include "PhysicsDebugDraw.h"
class RigidBody;
class PhysicsDebugDraw;

class CPhysicsWorld {
	btDefaultCollisionConfiguration* collisionConfig = nullptr;
	btCollisionDispatcher* collisionDispatcher = nullptr;
	btBroadphaseInterface* overlappingPairCache = nullptr;
	btSequentialImpulseConstraintSolver* constraintSolver = nullptr;
	btDiscreteDynamicsWorld*	 dynamicWorld = nullptr;		
	btIDebugDraw* debugDrawer = nullptr;
	PhysicsDebugDraw* m_debugDraw = nullptr;
	bool m_isDrawLine = false;
public:
	CPhysicsWorld();
	~CPhysicsWorld();
	void Init();
	void SetDebugDraw(bool flag) {
		m_isDrawLine = flag;
	}
	const bool IsDrawDebugLine() {
		return m_isDrawLine;
	}
	void Update();
	void DebugDraw();
	void Release();
	/*!
	* @brief	ダイナミックワールドを取得。
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return dynamicWorld;
	}
	/*!
	* @brief	剛体を登録。
	*/
	void AddRigidBody(RigidBody& rb);
	/*!
	* @brief	剛体を破棄。
	*/
	void RemoveRigidBody(RigidBody& rb);
	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}
	void ContactText(
		btCollisionObject* colObj,
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		dynamicWorld->contactTest(colObj, resultCallback);
	}
};
