#pragma once

#include "../graphics/Sprite/Sprite.h"
#include "../graphics/Camera.h"

class SpriteRender : public GameObject
{
public:
	SpriteRender();
	~SpriteRender();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	void ChangeCameraProjMatrix(Camera::EnUpdateProjMatrixFunc cameraMode) {
		m_sprite.ChangeCameraProjMatrix(cameraMode);
	}

	void SetMulCol(const CVector4& col) {
		m_sprite.SetMulColor(col);
	}

	void Init(const wchar_t* texFilePath, float w, float h,bool collisionFlag = false);

	void SetPosition(const CVector3& pos)
	{
		m_pos = pos;
		m_collider.Init(m_h, m_w, pos,m_pivot);
	}

	const CVector3& GetPosition() const
	{
		return m_pos;
	}

	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}

	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}

	void SetPivot(const CVector2& pivot)
	{
		m_pivot = pivot;
	}

	//毎フレーム呼び出してターゲット座標を更新する 
	void SetCollisionTarget(CVector3& tarPos) {
		m_target = tarPos;
	}

	bool  isCollidingTarget() {
		return m_collider.isColliding();
	}

	void SetScale(CVector3 scale) {
		m_scale = scale;
	}
private:
	CVector3 m_pos = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_pivot = Sprite::DEFAULT_PIVOT;
	bool m_isNeedExecuteCollision = false;
	SpriteBoxCollider m_collider;
	CVector3 m_target = CVector3::Zero();
	float m_w;
	float m_h;
	Sprite m_sprite;
};