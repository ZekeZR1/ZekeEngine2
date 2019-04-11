#include "stdafx.h"
#include "Engine/graphics/CCameraCollisionSlover.h"
#include "Engine/physics/CSphereCollider.h"

struct SConvexSweepCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	CVector3 m_rayDir;
	SConvexSweepCallback(CVector3 rayDir) :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f)),
		m_rayDir(rayDir) {}
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		CVector3 normal;
		normal.Set(convexResult.m_hitNormalLocal);

		if (convexResult.m_hitCollisionObject->getUserIndex() == 1) {
			return 1.0f;
		}
		return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}
};

CCameraCollisionSlover::CCameraCollisionSlover()
{
}


CCameraCollisionSlover::~CCameraCollisionSlover()
{
}
void CCameraCollisionSlover::Init(float radius)
{
	m_radius = radius;
	m_collider.Create(radius);
}
bool CCameraCollisionSlover::Execute(CVector3& result, const CVector3& position, const CVector3& target)
{
	result = position;
	CVector3 vWk;
	vWk.Subtract(target, position);
	if (vWk.LengthSq() < FLT_EPSILON) {
		return false;
	}
	vWk.Normalize();
	btTransform btStart, btEnd;
	btStart.setIdentity();
	btEnd.setIdentity();
	btStart.setOrigin(btVector3(target.x, target.y, target.z));
	btEnd.setOrigin(btVector3(position.x, position.y, position.z));
	SConvexSweepCallback callback(vWk);
	g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), btStart, btEnd, callback);
	if (callback.hasHit()) {
		CVector3 vHitPos;
		vHitPos.Set(callback.m_hitPointWorld);
		CVector3 vOffset;
		vOffset.Set(callback.m_hitNormalWorld);
		vOffset.Scale(m_radius);
		result.Add(vHitPos, vOffset);
		return true;
	}
	return false;
}