#pragma once
class SkinModelDummy : public GameObject
{
public:
	SkinModelDummy();
	~SkinModelDummy();
	bool Start() override;
	void Update() override;
	void Render() override;
	void Init(const wchar_t* filepath, EnFbxUpAxis);
	void SetPosition(CVector3 pos) {
		m_pos = pos;
	}
	void SetRotation(CQuaternion rot){
		m_rot = rot;
	}
	void CreatePhysicsStaticObject() {
		m_physicsStaticObject.CreateMeshObject(m_model, m_pos, m_rot);
	}
private:
	SkinModel m_model;
	CVector3 m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::Identity();
	PhysicsStaticObject m_physicsStaticObject;
};
