#include "stdafx.h"
#include "SpriteBoxCollider.h"

SpriteBoxCollider::SpriteBoxCollider()
{
}


SpriteBoxCollider::~SpriteBoxCollider()
{
}

void SpriteBoxCollider::Init(float height, float width, CVector3 pos,CVector2 pivot) 
{
	m_hitbox.pos = pos;
	m_hitbox.height = height;
	m_hitbox.width = width;

	m_pivot = pivot;
	SideCalc();
}

void SpriteBoxCollider::Execute(CVector3 tar) {
	if (tar.x < m_hitbox.rightSide &&
		m_hitbox.leftSide < tar.x &&
		tar.y >m_hitbox.downSide &&
		tar.y < m_hitbox.upperSide) {
		m_isColliding = true;
	}
	else {
		m_isColliding = false;
	}
}

void SpriteBoxCollider::SideCalc() {
	m_hitbox.upperSide = m_hitbox.pos.y + (m_hitbox.height * (1.f - m_pivot.y));
	m_hitbox.downSide = m_hitbox.pos.y - (m_hitbox.height * m_pivot.y);
	m_hitbox.rightSide = m_hitbox.pos.x + (m_hitbox.width * (1.f-m_pivot.x));
	m_hitbox.leftSide = m_hitbox.pos.x - (m_hitbox.width  * m_pivot.x);
}