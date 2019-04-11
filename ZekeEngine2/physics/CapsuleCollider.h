/*!
* @brief	カプセルコライダー。
*/

#pragma once

#include "Engine/Physics/ICollider.h"


class CapsuleCollider : public ICollider
{
public:
	/*!
	* @brief	デストラクタ。
	*/
	~CapsuleCollider();
	/*!
	* @brief	作成。
	*@param[in] radius	カプセルの半径。
	*@param[in]	height	カプセルの高さ。
	*/
	void Create(float radius, float height, EnFbxUpAxis axis)
	{
		m_axis = axis;
		if (axis == enFbxUpAxisY) {
			shape = new btCapsuleShape(radius, height);
		}
		else if(axis == enFbxUpAxisZ){
			shapez = new btCapsuleShapeZ(radius, height);
		}
		else {
			shapex = new btCapsuleShapeX(radius, height);
		}
		this->radius = radius;
		this->height = height;
	}
	btCollisionShape* GetBody() const override
	{
		if (m_axis == enFbxUpAxisY) {
			return shape;
		}
		else if (m_axis == enFbxUpAxisZ) {
			return shapez;
		}
		else {
			return shapex;
		}
	}
	float GetRadius() const
	{
		return radius;
	}
	float GetHeight() const
	{
		return height;
	}
private:
	btCapsuleShape * shape = nullptr;
	btCapsuleShapeZ * shapez = nullptr;
	btCapsuleShapeX * shapex = nullptr;
	EnFbxUpAxis m_axis;
	float radius = 0.0f;
	float height = 0.0f;
};
