#pragma once
class Goal;
class Stage : public GameObject
{
public:
	bool Start() override;;
	void OnDestroy() override;
	void Update() override;
	Goal* GetGoalObject() {
		return m_goal;
	}
private:
	SkinModelRender* m_floorModel = nullptr;
	CVector3 m_floorPos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::Identity();
	PhysicsStaticObject m_physicsStaticObject;
	Goal* m_goal = nullptr;
};

