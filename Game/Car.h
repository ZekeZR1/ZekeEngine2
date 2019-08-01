#pragma once
#include "MyVehicle.h"

class Car
{
public:
	Car();
	~Car();
	btDiscreteDynamicsWorld* getDynamicsWorld()
	{
		return m_dynamicsWorld;
	}
	void stepSimulation();
	void buttonUpdate();
	void ResetCar();

	void SetChassisPositionFix(CVector3 vec) {
		m_chassisPositionFix = vec;
	}
	void SetWheelModelPositionFix(CVector3 vec) {
		m_wheelPositionFix = vec;
	}
	void SetChassisCollisionShapeSize(CVector3 size) {
		m_chassisShapeSize = size;
	}

	CVector3 GetPosition() const{
		return m_vehicle->getChassisWorldTransform().getOrigin();
	}

	float GetSpeedKmHour() const{
		return m_vehicle->getCurrentSpeedKmHour();
	}

	CVector3 GetForwardVector() const {
		return m_vehicle->getForwardVector();
	}

private:
	void init();
	void modelInit();
	void modelUpdate();
	void Aerial(); //エアリアル制御
	//調整用
	//collider
	CVector3 m_chassisShapeSize = { 0.7,0.3,1.5 };
	float wheelRadius = 0.5f;
	float wheelWidth = 0.4f;
	//float wheelFriction = 1000;  //BT_LARGE_FLOAT;
	float wheelFriction = 10;  //BT_LARGE_FLOAT;
	//model
	CVector3 m_chassisPositionFix = CVector3::Zero();
	CVector3 m_chassisModelScale = CVector3::One();
	CVector3 m_wheelPositionFix = CVector3::Zero();
	CVector3 m_wheelModelScale = CVector3::One();

	btVector3 m_upVec;
	btVector3 m_forwardVec;
	btVector3 m_rightVec;


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


	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle = 0;
	//MyVehicle* m_vehicle = 0;
	btCollisionShape* m_wheelShape = 0;
	SkinModelRender* m_chassiModel = nullptr;
	SkinModelRender* m_frontLeftWheel = nullptr; 
	SkinModelRender* m_frontRightWheel = nullptr;
	SkinModelRender* m_rearRightWheel = nullptr;
	SkinModelRender* m_rearLeftWheel = nullptr;
	const float m_chassisMass = 800.f; //シャーシーの重さ
	const float normalMaxSpeed = 90.f;
	const float boostMaxSpeed = 100.f;
};

