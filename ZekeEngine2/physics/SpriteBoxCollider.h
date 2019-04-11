#pragma once
class SpriteBoxCollider
{
public:
	SpriteBoxCollider();
	~SpriteBoxCollider();
	/*!
		* @brief	�������B
		*@param[in]	height      �{�b�N�X�R���C�_�[�̍���
		*@param[in]	width		�{�b�N�X�R���C�_�[�̕�
		*@param[in]	position	�����ʒu(���S���W)
	*/
	void Init(float h, float w, CVector3 pos,CVector2 pivot);

	//�^�[�Q�b�g���W���X�V
	void Execute(CVector3 pos);

	bool isColliding() {
		return m_isColliding;
	}

private:
	struct SpriteHitBox {
		float height;
		float width;
		float upperSide;
		float downSide;
		float rightSide;
		float leftSide;
		CVector3 pos;
	};
	SpriteHitBox m_hitbox;
	CVector3 m_target;
	bool m_isColliding = false;
	void SideCalc();

	CVector2 m_pivot = {0.5f,0.5f };
};

