#pragma once
class Car
{
public:
	Car();
	~Car();
	btDiscreteDynamicsWorld* getDynamicsWorld()
	{
		return m_dynamicsWorld;
	}
	virtual void stepSimulation();
	virtual void buttonUpdate();
	void ResetCar();

	void SetChassisPositionFix(CVector3 vec) {
		m_chassisPositionFix = vec;
	}
	void SetChassisCollisionShapeSize(CVector3 size) {
		m_chassisShapeSize = size;
	}
private:
	void init();
	void modelUpdate();
	class btDiscreteDynamicsWorld* m_dynamicsWorld;
	btRigidBody* m_carChassis = 0;
	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colSape);

	int m_wheelInstances[4];
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

	class btBroadphaseInterface* m_overlappingPairCache;

	class btCollisionDispatcher* m_dispatcher;

	class btConstraintSolver* m_constraintSolver;

	class btDefaultCollisionConfiguration* m_collisionConfiguration;

	class btTriangleIndexVertexArray* m_indexVertexArrays = 0;

	float testParam = 0.f;

	btVector3* m_vertices = 0;
	CVector3 m_chassisShapeSize = { 0.7,0.3,1.5 };
	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle = 0;
	btCollisionShape* m_wheelShape = 0;
	CVector3 m_chassisPositionFix = CVector3::Zero();
	SkinModelRender* m_chassiModel = nullptr;
	SkinModelRender* m_frontLeftWheel = nullptr; 
	SkinModelRender* m_frontRightWheel = nullptr;
	SkinModelRender* m_rearRightWheel = nullptr;
	SkinModelRender* m_rearLeftWheel = nullptr;

};

