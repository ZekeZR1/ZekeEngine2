#pragma once
class SpriteBoxCollider
{
public:
	SpriteBoxCollider();
	~SpriteBoxCollider();
	/*!
		* @brief	初期化。
		*@param[in]	height      ボックスコライダーの高さ
		*@param[in]	width		ボックスコライダーの幅
		*@param[in]	position	初期位置(中心座標)
	*/
	void Init(float h, float w, CVector3 pos,CVector2 pivot);

	//ターゲット座標を更新
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

