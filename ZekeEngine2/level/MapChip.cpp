#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"

MapChip::MapChip(const LevelObjectData& objData)
{
	wchar_t filePath[256];
	swprintf_s(filePath, L"Assets/modelData/%s.cmo", objData.name);
	m_model.Init(filePath);
	m_model.UpdateWorldMatrix(objData.position, objData.rotation, CVector3::One());
	//静的物理オブジェクトをメッシュコライダーから作成する。
	m_physicsStaticObject.CreateMeshObject(m_model, objData.position, objData.rotation);
}

void MapChip::Draw()
{
	extern Camera*  camera3d;
	m_model.Draw(camera3d->GetViewMatrix(),camera3d->GetProjectionMatrix());
}