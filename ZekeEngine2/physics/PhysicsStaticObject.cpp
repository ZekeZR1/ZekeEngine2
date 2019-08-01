/*!
* @brief	静的オブジェクト。
*/

#include "stdafx.h"
#include "PhysicsStaticObject.h"


PhysicsStaticObject::PhysicsStaticObject()
{
}
PhysicsStaticObject::~PhysicsStaticObject()
{
	PhysicsWorld().RemoveRigidBody(m_rigidBody);
}

void PhysicsStaticObject::CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot, float restitution)
{
	//メッシュコライダーを作成。
	m_meshCollider.CreateFromSkinModel(skinModel, nullptr);
	//剛体を作成、
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider; //剛体に形状(コライダー)を設定する。
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	m_rigidBody.GetBody()->setRestitution(restitution);
	//剛体を物理ワールドに追加する。
	PhysicsWorld().AddRigidBody(m_rigidBody);
}


//
//void PhysicsStaticObject::CreateMeshObject(SkinModelRender& skinModelRender, CVector3 pos, CQuaternion rot)
//{
//	//メッシュコライダーを作成。
//	auto model = skinModelRender.GetSkinModel();
//	m_meshCollider.CreateFromSkinModel(model, nullptr);
//	//剛体を作成、
//	RigidBodyInfo rbInfo;
//	rbInfo.collider = &m_meshCollider; //剛体に形状(コライダー)を設定する。
//	rbInfo.mass = 0.0f;
//	rbInfo.pos = pos;
//	rbInfo.rot = rot;
//	m_rigidBody.Create(rbInfo);
//	//剛体を物理ワールドに追加する。
//	PhysicsWorld().AddRigidBody(m_rigidBody);
//}


