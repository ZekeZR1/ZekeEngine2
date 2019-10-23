#include "stdafx.h"
#include "RigitBody.h"
#include "ICollider.h"


RigidBody::~RigidBody()
{
	Release();
}
void RigidBody::Release()
{
	delete rigidBody;
	delete myMotionState;
	rigidBody = nullptr;
	myMotionState = nullptr;
}
void RigidBody::Create(RigidBodyInfo& rbInfo)
{

	assert((!rbInfo.collider->GetBody() || rbInfo.collider->GetBody()->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (rbInfo.mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		rbInfo.collider->GetBody()->calculateLocalInertia(rbInfo.mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btTransform startTransform;
	startTransform.setOrigin(btVector3(
	btScalar(rbInfo.pos.x),
	btScalar(rbInfo.pos.y),
	btScalar(rbInfo.pos.z)));

	btDefaultMotionState * myMotionState = new btDefaultMotionState();

	btRigidBody::btRigidBodyConstructionInfo cInfo(rbInfo.mass, myMotionState, rbInfo.collider->GetBody(), localInertia);

	btRigidBody * body = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
	btRigidBody * body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif  //

	body->setUserIndex(-1);

	rigidBody = body;
}
