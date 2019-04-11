#include "stdafx.h"
#include "SpriteRender.h"



SpriteRender::SpriteRender()
{
}


SpriteRender::~SpriteRender()
{
}


bool SpriteRender::Start() {
	return true;
}

void SpriteRender::Init(const wchar_t* FilePath, float w, float h, bool Collisionflag) {
	m_sprite.Init(FilePath, w, h);
	m_w = w;
	m_h = h;
	if (Collisionflag) {
		m_isNeedExecuteCollision = true;
	}
}

void SpriteRender::Update() {
	if (m_isNeedExecuteCollision) {
		m_collider.Execute(m_target);
	}
	m_sprite.Update(m_pos, m_rotation, m_scale, m_pivot);
}

void SpriteRender::PostRender() {
	m_sprite.Draw();
}

