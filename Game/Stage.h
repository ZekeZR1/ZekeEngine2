#pragma once
class Stage : public GameObject
{
public:
	bool Start() override;;
	void OnDestroy() override;
	void Update() override;
private:
	SkinModelRender* m_floorModel = nullptr;
	CVector3 m_floorPos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::Identity();
	PhysicsStaticObject m_physicsStaticObject;
};

