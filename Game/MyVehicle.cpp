#include "stdafx.h"
#include "MyVehicle.h"

#include "BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h"
#include "BulletDynamics/ConstraintSolver/btJacobianEntry.h"
#include "LinearMath/btQuaternion.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../ZekeEngine2/bulletPhysics/src/BulletDynamics/Vehicle/btVehicleRaycaster.h"
#include "../ZekeEngine2/bulletPhysics/src/BulletDynamics/Vehicle/btWheelInfo.h"
#include "LinearMath/btMinMax.h"
#include "LinearMath/btIDebugDraw.h"
#include "BulletDynamics/ConstraintSolver/btContactConstraint.h"

//
//void MyVehicle::updateFriction(btScalar	timeStep) {
//	//calculate the impulse, so that the wheels don't move sidewards
//	int numWheel = getNumWheels();
//	if (!numWheel)
//		return;
//
//	m_forwardWS.resize(numWheel);
//	m_axle.resize(numWheel);
//	m_forwardImpulse.resize(numWheel);
//	m_sideImpulse.resize(numWheel);
//
//	int numWheelsOnGround = 0;
//
//
//	//collapse all those loops into one!
//	for (int i = 0; i < getNumWheels(); i++)
//	{
//		btWheelInfo& wheelInfo = m_wheelInfo[i];
//		class btRigidBody* groundObject = (class btRigidBody*) wheelInfo.m_raycastInfo.m_groundObject;
//		if (groundObject)
//			numWheelsOnGround++;
//		m_sideImpulse[i] = btScalar(0.);
//		m_forwardImpulse[i] = btScalar(0.);
//
//	}
//
//	{
//
//		for (int i = 0; i < getNumWheels(); i++)
//		{
//
//			btWheelInfo& wheelInfo = m_wheelInfo[i];
//
//			class btRigidBody* groundObject = (class btRigidBody*) wheelInfo.m_raycastInfo.m_groundObject;
//
//			if (groundObject)
//			{
//
//				const btTransform& wheelTrans = getWheelTransformWS(i);
//
//				btMatrix3x3 wheelBasis0 = wheelTrans.getBasis();
//				m_axle[i] = btVector3(
//					wheelBasis0[0][m_indexRightAxis],
//					wheelBasis0[1][m_indexRightAxis],
//					wheelBasis0[2][m_indexRightAxis]);
//
//				const btVector3& surfNormalWS = wheelInfo.m_raycastInfo.m_contactNormalWS;
//				btScalar proj = m_axle[i].dot(surfNormalWS);
//				m_axle[i] -= surfNormalWS * proj;
//				m_axle[i] = m_axle[i].normalize();
//
//				m_forwardWS[i] = surfNormalWS.cross(m_axle[i]);
//				m_forwardWS[i].normalize();
//
//				m_chassisBody = getRigidBody();
//				resolveSingleBilateral(*m_chassisBody, wheelInfo.m_raycastInfo.m_contactPointWS,
//					*groundObject, wheelInfo.m_raycastInfo.m_contactPointWS,
//					btScalar(0.), m_axle[i], m_sideImpulse[i], timeStep);
//
//				m_sideImpulse[i] *= sideFrictionStiffness2;
//
//			}
//
//
//		}
//	}
//
//	btScalar sideFactor = btScalar(1.);
//	btScalar fwdFactor = 0.5;
//
//	bool sliding = false;
//	{
//		for (int wheel = 0; wheel < getNumWheels(); wheel++)
//		{
//			btWheelInfo& wheelInfo = m_wheelInfo[wheel];
//			class btRigidBody* groundObject = (class btRigidBody*) wheelInfo.m_raycastInfo.m_groundObject;
//
//			btScalar	rollingFriction = 0.f;
//
//			if (groundObject)
//			{
//				if (wheelInfo.m_engineForce != 0.f)
//				{
//					rollingFriction = wheelInfo.m_engineForce * timeStep;
//				}
//				else
//				{
//					btScalar defaultRollingFrictionImpulse = 0.f;
//					btScalar maxImpulse = wheelInfo.m_brake ? wheelInfo.m_brake : defaultRollingFrictionImpulse;
//					btWheelContactPoint contactPt(m_chassisBody, groundObject, wheelInfo.m_raycastInfo.m_contactPointWS, m_forwardWS[wheel], maxImpulse);
//					rollingFriction = calcRollingFriction(contactPt);
//				}
//			}
//
//			//switch between active rolling (throttle), braking and non-active rolling friction (no throttle/break)
//
//
//
//
//			m_forwardImpulse[wheel] = btScalar(0.);
//			m_wheelInfo[wheel].m_skidInfo = btScalar(1.);
//
//			if (groundObject)
//			{
//				m_wheelInfo[wheel].m_skidInfo = btScalar(1.);
//
//				btScalar maximp = wheelInfo.m_wheelsSuspensionForce * timeStep * wheelInfo.m_frictionSlip;
//				btScalar maximpSide = maximp;
//
//				btScalar maximpSquared = maximp * maximpSide;
//
//
//				m_forwardImpulse[wheel] = rollingFriction;//wheelInfo.m_engineForce* timeStep;
//
//				btScalar x = (m_forwardImpulse[wheel]) * fwdFactor;
//				btScalar y = (m_sideImpulse[wheel]) * sideFactor;
//
//				btScalar impulseSquared = (x * x + y * y);
//
//				if (impulseSquared > maximpSquared)
//				{
//					sliding = true;
//
//					btScalar factor = maximp / btSqrt(impulseSquared);
//
//					m_wheelInfo[wheel].m_skidInfo *= factor;
//				}
//			}
//
//		}
//	}
//
//
//
//
//	if (sliding)
//	{
//		for (int wheel = 0; wheel < getNumWheels(); wheel++)
//		{
//			if (m_sideImpulse[wheel] != btScalar(0.))
//			{
//				if (m_wheelInfo[wheel].m_skidInfo < btScalar(1.))
//				{
//					m_forwardImpulse[wheel] *= m_wheelInfo[wheel].m_skidInfo;
//					m_sideImpulse[wheel] *= m_wheelInfo[wheel].m_skidInfo;
//				}
//			}
//		}
//	}
//
//	// apply the impulses
//	{
//		for (int wheel = 0; wheel < getNumWheels(); wheel++)
//		{
//			btWheelInfo& wheelInfo = m_wheelInfo[wheel];
//
//			btVector3 rel_pos = wheelInfo.m_raycastInfo.m_contactPointWS -
//				m_chassisBody->getCenterOfMassPosition();
//
//			if (m_forwardImpulse[wheel] != btScalar(0.))
//			{
//				m_chassisBody->applyImpulse(m_forwardWS[wheel] * (m_forwardImpulse[wheel]), rel_pos);
//			}
//			if (m_sideImpulse[wheel] != btScalar(0.))
//			{
//				class btRigidBody* groundObject = (class btRigidBody*) m_wheelInfo[wheel].m_raycastInfo.m_groundObject;
//
//				btVector3 rel_pos2 = wheelInfo.m_raycastInfo.m_contactPointWS -
//					groundObject->getCenterOfMassPosition();
//
//
//				btVector3 sideImp = m_axle[wheel] * m_sideImpulse[wheel];
//
//#if defined ROLLING_INFLUENCE_FIX // fix. It only worked if car's up was along Y - VT.
//				btVector3 vChassisWorldUp = getRigidBody()->getCenterOfMassTransform().getBasis().getColumn(m_indexUpAxis);
//				rel_pos -= vChassisWorldUp * (vChassisWorldUp.dot(rel_pos) * (1.f - wheelInfo.m_rollInfluence));
//#else
//				rel_pos[m_indexUpAxis] *= wheelInfo.m_rollInfluence;
//#endif
//				m_chassisBody->applyImpulse(sideImp, rel_pos);
//
//				//apply friction impulse on the ground
//				groundObject->applyImpulse(-sideImp, rel_pos2);
//			}
//		}
//	}
//}