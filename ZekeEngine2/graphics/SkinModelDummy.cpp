#include "stdafx.h"
#include "SkinModelDummy.h"



SkinModelDummy::SkinModelDummy()
{
}


SkinModelDummy::~SkinModelDummy()
{
}


bool SkinModelDummy::Start() {
	return true;
}

void SkinModelDummy::Update() {
	m_model.UpdateWorldMatrix(m_pos, m_rot, CVector3::One());
}

void SkinModelDummy::Render() {

}

void SkinModelDummy::Init(const wchar_t* filepath, EnFbxUpAxis axis) {
	m_model.Init(filepath, axis);
}
