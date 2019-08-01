#pragma once
//class MyVehicle :
//	public btRaycastVehicle
//{
//
//	btAlignedObjectArray<btVector3>	m_forwardWS;
//	btAlignedObjectArray<btVector3>	m_axle;
//	btAlignedObjectArray<btScalar>	m_forwardImpulse;
//	btAlignedObjectArray<btScalar>	m_sideImpulse;
//
//public:
//	using btRaycastVehicle::btRaycastVehicle;
//	//btRaycastVehicle::btRaycastVehicle(const btVehicleTuning& tuning, btRigidBody* chassis, btVehicleRaycaster* raycaster)
//	void	updateFriction(btScalar	timeStep) override;
//private:
//	btRigidBody* m_chassisBody = nullptr;
//	int m_indexUpAxis;
//	btScalar sideFrictionStiffness2 = btScalar(1.0);
//
//	int m_indexRightAxis = 0;
//	int m_indexUpAxis = 2;
//	int m_indexForwardAxis = 1;
//
//	struct btWheelContactPoint
//	{
//		btRigidBody* m_body0;
//		btRigidBody* m_body1;
//		btVector3	m_frictionPositionWorld;
//		btVector3	m_frictionDirectionWorld;
//		btScalar	m_jacDiagABInv;
//		btScalar	m_maxImpulse;
//
//
//		btWheelContactPoint(btRigidBody* body0, btRigidBody* body1, const btVector3& frictionPosWorld, const btVector3& frictionDirectionWorld, btScalar maxImpulse)
//			:m_body0(body0),
//			m_body1(body1),
//			m_frictionPositionWorld(frictionPosWorld),
//			m_frictionDirectionWorld(frictionDirectionWorld),
//			m_maxImpulse(maxImpulse)
//		{
//			btScalar denom0 = body0->computeImpulseDenominator(frictionPosWorld, frictionDirectionWorld);
//			btScalar denom1 = body1->computeImpulseDenominator(frictionPosWorld, frictionDirectionWorld);
//			btScalar	relaxation = 1.f;
//			m_jacDiagABInv = relaxation / (denom0 + denom1);
//		}
//	};
//
//	btScalar calcRollingFriction(btWheelContactPoint & contactPoint)
//	{
//
//		btScalar j1 = 0.f;
//
//		const btVector3& contactPosWorld = contactPoint.m_frictionPositionWorld;
//
//		btVector3 rel_pos1 = contactPosWorld - contactPoint.m_body0->getCenterOfMassPosition();
//		btVector3 rel_pos2 = contactPosWorld - contactPoint.m_body1->getCenterOfMassPosition();
//
//		btScalar maxImpulse = contactPoint.m_maxImpulse;
//
//		btVector3 vel1 = contactPoint.m_body0->getVelocityInLocalPoint(rel_pos1);
//		btVector3 vel2 = contactPoint.m_body1->getVelocityInLocalPoint(rel_pos2);
//		btVector3 vel = vel1 - vel2;
//
//		btScalar vrel = contactPoint.m_frictionDirectionWorld.dot(vel);
//
//		// calculate j that moves us to zero relative velocity
//		j1 = -vrel * contactPoint.m_jacDiagABInv;
//		btSetMin(j1, maxImpulse);
//		btSetMax(j1, -maxImpulse);
//
//		return j1;
//	}
//
//
//};
//
