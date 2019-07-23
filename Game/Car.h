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
private:
	void init();
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

	btVector3* m_vertices = 0;

	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle = 0;
	btCollisionShape* m_wheelShape = 0;

	SkinModelRender* m_chassiModel = nullptr;
	SkinModelRender* m_frontLeftWheel = nullptr; 
	SkinModelRender* m_frontRightWheel = nullptr;
	SkinModelRender* m_rearRightWheel = nullptr;
	SkinModelRender* m_rearLeftWheel = nullptr;

};

