#pragma once

#include "../graphics/Sprite/Sprite.h"
#include "../graphics/Camera.h"
class FontRender;
class SpriteRender : public GameObject
{
public:
	SpriteRender();
	~SpriteRender();
	bool Start() override;
	void Update() override;
	void Render() override;
	void PostRender() override;
	void ChangeCameraProjMatrix(Camera::EnUpdateProjMatrixFunc cameraMode) {
		m_sprite.ChangeCameraProjMatrix(cameraMode);
		m_projMatrixFunc = cameraMode;
	}

	void SetMulCol(const CVector4& col) {
		m_sprite.SetMulColor(col);
	}


	void Init(const wchar_t* texFilePath, float w, float h, bool collisionFlag = true, bool PosChangeByMouse = false);

	void SetPosition(const CVector3& pos)
	{
		m_pos = pos;
		m_collider.Init(m_h, m_w, pos, m_pivot);
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
		m_collider.Init(m_h, m_w, m_pos, m_pivot);
	}

	//���t���[���Ăяo���ă^�[�Q�b�g���W���X�V���� 
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
	//���������Ƀ}�E�X�ɂ�钲���t���O��true�ɂ���ƈȉ��̑��삪�ł���.
	//�h���b�O : �|�W�V�����ړ�.
	//�}�E�X�z�C�[��  : Z���W�ړ�.
	//�E�N���b�N�������Ȃ���}�E�X�z�C�[�� : �X�P�[���ύX.
	//���Q�[�����݂̂ŕύX���K�������̂Œ��������������ꍇ�v���O������ύX����K�v������܂�.
	//�����̋@�\���g�p����ɂ̓X�v���C�g�̓����蔻��t���O��true�ɂ���K�v������܂�.
	void MouseActionUpdate();

	Camera::EnUpdateProjMatrixFunc m_projMatrixFunc = Camera::enUpdateProjMatrixFunc_Ortho;
	CVector3 m_pos = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_pivot = Sprite::DEFAULT_PIVOT;
	bool m_isNeedExecuteCollision = false;
	bool m_isPosChangeByMouse = false;
	FontRender* m_PosDataText = nullptr;
	SpriteBoxCollider m_collider;
	CVector3 m_target = CVector3::Zero();
	float m_w;
	float m_h;
	Sprite m_sprite;
};