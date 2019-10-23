#include "stdafx.h"
#include "Ball.h"
#include "..//ZekeEngine2/physics/CSphereCollider.h"

void Ball::Awake() {
	m_ballModel = NewGO<SkinModelRender>(0,"Ball");
	m_ballModel->Init(L"Assets/modelData/ball.cmo");
}

bool Ball::Start() {
	btVector3 localInertia(0, 0, 0);

	m_col.Create(m_radius);
	m_col.GetBody()->calculateLocalInertia(m_mass, localInertia);

	RigidBodyInfo rbinfo;
	rbinfo.pos = { 0,10,0 };
	rbinfo.mass = m_mass;
	rbinfo.collider = &m_col;

	m_rb.Create(rbinfo);

	m_rb.GetBody()->applyDamping(m_damping);
	m_rb.GetBody()->setRestitution(m_restitution);

	PhysicsWorld().AddRigidBody(m_rb);

	return true;
}

void Ball::OnDestroy() {
	PhysicsWorld().RemoveRigidBody(m_rb);
	DeleteGO(m_ballModel);
}

void Ball::Update() {
	m_ballModel->SetPosition(m_rb.GetBody()->getWorldTransform().getOrigin());
	m_ballModel->SetRotation(m_rb.GetBody()->getWorldTransform().getRotation());
}

void Ball::ResetBall() {
	btTransform wtr;
	wtr.setOrigin(btVector3(0, 5, 0));
	wtr.setRotation(btQuaternion::getIdentity());
	m_rb.GetBody()->setCenterOfMassTransform(wtr);
	m_rb.GetBody()->setLinearVelocity(btVector3(0, 0, 0));
	m_rb.GetBody()->setAngularVelocity(btVector3(0, 0, 0));
}

void Ball::SetTransform(CVector3 pos, CQuaternion rot) {
	btTransform wtr;
	wtr.setOrigin(pos);
	wtr.setRotation(rot);
	if(m_rb.GetBody() != nullptr)
		m_rb.GetBody()->setCenterOfMassTransform(wtr);
}
