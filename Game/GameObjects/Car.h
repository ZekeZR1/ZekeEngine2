#pragma once
class CarState;

class Car : public GameObject
{
public:
	Car();
	~Car();

	bool Start() override;
	void OnDestroy() override;
	void Update() override;

	enum eState {
		enOnGround,
		enInAir,
		enFlip,
		enNumState
	};

	/// <summary>
	/// 車制御用のパラメータ構造体
	/// </summary>
	struct CarControll {
		float accel = 0.f;
		float back = 0.f;
		float steering = 0.f;
		float aerealX = 0.f;
		float aerealY = 0.f;
		bool jump = false;
		bool boost = false;
		bool airRoll = false;
	};
	/// <summary>
	/// 車のステート返す
	/// </summary>
	/// <param name="state">インスタンスを取得したい状態</param>
	/// <returns>新しいステートのインスタンス</returns>
	CarState* GetCarState(eState state) {
		return m_StatePool.at(state);
	}

	void stepSimulation();
	void buttonUpdate();
	void ResetCar(CVector3 resetPos = { 0,2,-10 }, CQuaternion resetQuaternion = CQuaternion::Identity());

	void SetTransform(CVector3 pos, CQuaternion rot = CQuaternion::Identity());

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

	CQuaternion GetRotation() const {
		return m_vehicle->getChassisWorldTransform().getRotation();
	}

	float GetSpeedKmHour() const{
		return m_vehicle->getCurrentSpeedKmHour();
	}

	CVector3 GetForwardVector() const {
		return m_vehicle->getForwardVector();
	}

	/// <summary>
	/// 4つすべてのタイヤが地面についているかの判定結果を返します
	/// </summary>
	/// <returns></returns>
	bool isOnGround() {
		return m_isOnGround;
	}
	/// <summary>
	/// bulletのraycast車のポインタを返します
	/// </summary>
	/// <returns></returns>
	btRaycastVehicle* GetRayCastVehicle() {
		return m_vehicle;
	}

	/// <summary>
	/// エンジンの出力をセットする
	/// </summary>
	/// <param name="force"></param>
	void SetEngineForce(float force) {
		m_engineForce = force;
	}

	/// <summary>
	/// ブレーキ
	/// </summary>
	/// <param name="force">ブレーキ力</param>
	void SetBreakingForce(float force) {
		m_breakingForce = force;
	}

	/// <summary>
	/// ハンドルを切る
	/// </summary>
	/// <param name="steering">ステアリングパラメータ</param>
	void SetSteering(float steering) {
		m_vehicleSteering = steering;
	}
	/// <summary>
	/// 車の前方向を取得
	/// </summary>
	/// <returns>正規化された前方向のベクトルを返す</returns>
	CVector3 GetCarForward() {
		return m_forwardVec;
	}
	/// <summary>
	/// 車の右方向を取得
	/// </summary>
	/// <returns>正規化された右方向のベクトルを返す</returns>
	CVector3 GetCarRight() {
		return m_rightVec;
	}
	/// <summary>
	/// 車の上方向を取得
	/// </summary>
	/// <returns>正規化された上方向のベクトルを返す</returns>
	btVector3 GetCarUp() {
		return m_upVec;
	}

	/// <summary>
	/// 車制御用のインプット構造体をセットする
	/// </summary>
	/// <param name="input">コントローラーの入力状態を適用した構造体</param>
	void SetCarInput(CarControll& input);

	/// <summary>
	/// ローカルプレイヤーの車かどうかセットする
	/// </summary>
	/// <param name="flag">ローカルプレイヤーの車か</param>
	void SetHostCarFlag(bool flag) {
		m_isHostPlayerCar = flag;
	}

private:
	void init();
	void modelInit();
	void modelUpdate();
	void MoveLimit();
	//調整用
	//collider
	float connectionHeight = 0.2f; //ホイールを接続する高さ。
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

	float testParam = 0.f;

	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle = 0;
	//MyVehicle* m_vehicle = 0;
	btCollisionShape* m_wheelShape = 0;
	std::list<SkinModelRender*> m_models;
	SkinModelRender* m_chassiModel = nullptr;
	SkinModelRender* m_frontLeftWheel = nullptr; 
	SkinModelRender* m_frontRightWheel = nullptr;
	SkinModelRender* m_rearRightWheel = nullptr;
	SkinModelRender* m_rearLeftWheel = nullptr;
	const float m_chassisMass = 800.f; //シャーシーの重さ
	//const float normalMaxSpeed = 90.f;
	//const float boostMaxSpeed = 100.f;
	bool m_isOnGround = true;

	float m_engineForce = 0.f; //エンジンの出力
	float m_vehicleSteering = 0.f; //ステアリング
	float m_breakingForce = 100.f; //ブレーキ


	CarState* m_state = nullptr;
	std::vector<CarState*> m_StatePool;
	CarControll m_carInputs;

	bool m_isHostPlayerCar = false;
};

