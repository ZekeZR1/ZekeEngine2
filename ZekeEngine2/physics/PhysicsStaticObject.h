/*!
* @brief	静的オブジェクト。
*/

#pragma once

#include "MeshCollider.h"
#include "RigitBody.h"

/*!
* @brief	静的物理オブジェクト
*/
class PhysicsStaticObject {
public:
	/*!
	* @brief	コンストラクタ。
	*/
	PhysicsStaticObject();
	/*!
	* @brief	デストラクタ。
	*/
	~PhysicsStaticObject();
	/*!
	* @brief	メッシュの静的オブジェクトを作成。
	*@param[in]	skinModel	スキンモデル。
	*@param[in]	pos			座標。
	*@param[in]	rot			回転。
	*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot,float restitution = 0.f);
private:
	MeshCollider m_meshCollider;		//!<メッシュコライダー。
	RigidBody m_rigidBody;				//!<剛体。
};
