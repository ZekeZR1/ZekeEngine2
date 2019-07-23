#pragma once


//#include "..//ZekeEngine2/physics/RigitBody.h"

class Vehicle
{
public:
	Vehicle();
	~Vehicle();

	void init();
	void stepSimulation();
	void KeyUpdate();

	bool IsOnGround() const
	{
		return m_isOnGround;
	}
private:
	void RemoveRigidBoddy();

	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colSape);
	btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color = btVector4(1, 0, 0, 1));

	CVector3 m_positiron = CVector3::Zero();
	bool				m_isOnGround = true;		
	//CapsuleCollider		m_collider;	
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

	btRigidBody* m_carChassis;
	int m_wheelInstances[4];

	//RigidBody m_rigidBody;
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
	float gEngineForce = 0.f;
	float defaultBreakingForce = 10.f;
	float gBreakingForce = 100.f;
	float maxEngineForce = 1000.f;  //this should be engine/velocity dependent
	float gVehicleSteering = 0.f;
	float steeringIncrement = 0.04f;
	float steeringClamp = 0.3f;
	float wheelRadius = 0.5f;
	float wheelWidth = 0.4f;
	btCollisionShape* m_wheelShape = 0;

	//model
	std::vector<SkinModelRender*> m_carModels;
	// btWheelInfo is the main struct for defining suspension & wheel parameters
	struct btWheelInfo
	{
		// RaycastInfo contains info for raycasting the wheel.
		// These are updated by Bullet
		struct RaycastInfo
		{
			/// The normal at the ray contact point (world space)
			btVector3 m_contactNormalWS;
			/// The position of contact of the ray (world space)
			btVector3 m_contactPointWS;
			/// The current length of the suspension (metres)
			btScalar m_suspensionLength;
			/// The starting point of the raycast, where the suspension connects to the chassis (world space) (= chassisTransform * m_chassisConnectionPointCS)
			btVector3 m_hardPointWS;
			/// The direction of ray cast (in world space) (= chassisTransform * m_wheelDirectionCS)
			/// The wheel moves relative to the chassis in this direction, and the suspension force acts along this direction.
			btVector3 m_wheelDirectionWS;
			/// The direction of the wheel's axle (world space) (= chassisTransform * m_wheelAxleCS)
			/// The wheel rotates around this axis
			btVector3 m_wheelAxleWS;
			/// True if the wheel is in contact with something (= m_groundObject != NULL)
			bool m_isInContact;
			/// The other object the wheel is in contact with (btCollisionObject*)
			void* m_groundObject;
		};

		RaycastInfo m_raycastInfo;

		/// The wheel's world transform
		btTransform m_worldTransform;

		/// The starting point of the ray, where the suspension connects to the chassis (chassis space) (see also: m_raycastInfo.m_hardPointWS)
		btVector3 m_chassisConnectionPointCS;
		/// The direction of ray cast (chassis space) (see also: m_raycastInfo.m_wheelDirectionWS)
		btVector3 m_wheelDirectionCS;
		/// The axis the wheel rotates around (chassis space) (see also: m_raycastInfo.m_wheelAxleWS)
		btVector3 m_wheelAxleCS;
		/// The maximum length of the suspension (metres)
		btScalar m_suspensionRestLength1;
		/// The maximum distance the suspension can be compressed (centimetres)
		btScalar m_maxSuspensionTravelCm;
		// The radius of the wheel
		btScalar m_wheelsRadius;
		// The stiffness constant for the suspension.  10.0 - Offroad buggy, 50.0 - Sports car, 200.0 - F1 Car
		btScalar m_suspensionStiffness;
		// The damping coefficient for when the suspension is compressed. Set to k * 2.0 * btSqrt(m_suspensionStiffness) so k is proportional to critical damping.
		// k = 0.0 undamped & bouncy, k = 1.0 critical damping
		// k = 0.1 to 0.3 are good values
		btScalar m_wheelsDampingCompression;
		// The damping coefficient for when the suspension is expanding.  See the comments for m_wheelsDampingCompression for how to set k.
		// m_wheelsDampingRelaxation should be slightly larger than m_wheelsDampingCompression, eg k = 0.2 to 0.5
		btScalar m_wheelsDampingRelaxation;
		// The coefficient of friction between the tyre and the ground.
		// Should be about 0.8 for realistic cars, but can increased for better handling.
		// Set large (10000.0) for kart racers
		btScalar m_frictionSlip;
		// Set the angle of the wheels relative to the vehicle. (radians)
		btScalar m_steering;
		// The rotation of the wheel around it's axle (output radians.)
		btScalar m_rotation;
		// The amount of rotation around the wheel's axle this frame. (output radians)
		btScalar m_deltaRotation;
		// Reduces the rolling torque applied from the wheels that cause the vehicle to roll over.
		// This is a bit of a hack, but it's quite effective. 0.0 = no roll, 1.0 = physical behaviour.
		// If m_frictionSlip is too high, you'll need to reduce this to stop the vehicle rolling over.
		// You should also try lowering the vehicle's centre of mass
		btScalar m_rollInfluence;
		// Amount of torque applied to the wheel.
		// This provides the vehicle's acceleration
		btScalar m_engineForce;
		// Amount of braking torque applied to the wheel.
		btScalar m_brake;
		// Set to true if the wheel is a front wheel.
		// You can use this to select to apply either engine force or steering.
		bool m_bIsFrontWheel;
		// A handy place to stash a pointer to your own structures.
		void* m_clientInfo;

		// An internal suspension used to modify the suspension forces by the contact normal
		btScalar m_clippedInvContactDotSuspension;
		/// Output: the velocity of the wheel relative to the chassis.
		btScalar m_suspensionRelativeVelocity;
		/// Output: the force applied to the chassis by the suspension.
		btScalar m_wheelsSuspensionForce;
		/// Output: the amount of grip the wheels have with the driving surface.
		/// 0.0 = wheels are sliding, 1.0 = wheels have traction.
		/// Use to trigger sliding sounds, dust trails, skid marks etc.
		btScalar m_skidInfo;

	};
};

