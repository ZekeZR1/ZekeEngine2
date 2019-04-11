/*!
* @brief	カプセルコライダー。
*/

#include "stdafx.h"
#include "CapsuleCollider.h"


/*!
* @brief	デストラクタ。
*/
CapsuleCollider::~CapsuleCollider()
{
	if (m_axis == enFbxUpAxisY) {
		delete shape;
	}
	else if (m_axis == enFbxUpAxisZ) {
		delete shapez;
	}
	else {
		delete shapex;
	}
}
