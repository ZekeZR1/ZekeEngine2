#include "stdafx.h"
#include "Ball.h"
#include "..//ZekeEngine2/physics/CSphereCollider.h"

void Ball::Awake() {
	m_ballModel = NewGO<SkinModelRender>(0);
	m_ballModel->Init(L"Assets/modelData/ball.cmo");
}

bool Ball::Start() {
	
	//btSphereShape* colShape  = new btSphereShape(m_radius);
	btSphereShape* colShape = new btSphereShape(m_radius);
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.1f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(
		btScalar(0),
		btScalar(20),
		btScalar(0)));
	 m_rigidBody = createRigidBody(mass, startTransform, colShape);
	 m_rigidBody->applyDamping(-2.f);
	return true;
}

void Ball::OnDestroy() {

}

void Ball::Update() {
	m_ballModel->SetPosition(m_rigidBody->getWorldTransform().getOrigin());
}

