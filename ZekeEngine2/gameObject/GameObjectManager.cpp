#include "stdafx.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "CTransform.h"

void GameObjectManager::Execute()
{
	ExecuteDeleteGameObjects();

	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->StartWrapper();
		}
	}
	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->PreUpdateWrapper();
		}
	}

	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->UpdateWrapper();
		}
	}


	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->PostUpdateWrapper();
		}
	}

	//シャドウマップを更新。
	m_shadowMap.UpdateFromLightTarget(
		{ 1000.0f, 1000.0f, 1000.0f },
		{ 0.0f, 0.0f, 0.0f }
	);

	m_postEffect.Update();

	g_graphicsEngine->GetEffectEngine().Update();

	g_graphicsEngine->BegineRender();
	//フレームバッファののレンダリングターゲットをバックアップしておく。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
	//シャドウマップにレンダリング
	m_shadowMap.RenderToShadowMap();
	//レンダリングターゲットをメインに変更する。
	g_graphicsEngine->ChangeRenderTarget(&m_mainRenderTarget, &m_frameBufferViewports);
	//メインレンダリングターゲットをクリアする。
	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
	//TODO :  深度drawして取得できるようにする
	//レンダリング
	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->PreRenderWrapper();
		}
	}

	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->RenderWrapper();
		}
	}

	g_graphicsEngine->GetEffectEngine().Draw();

	for (GameObjectList objList : m_gameObjectListArray) {
		for (GameObject* obj : objList) {
			obj->PostRenderWrapper();
		}
	}

	m_postEffect.Draw();
	//physics
#if _DEBUG
	if(g_physics.IsDrawDebugLine())
		g_physics.DebugDraw();
#endif
	//g_physics.GetDynamicWorld()->debugDrawWorld();

	//レンダリングターゲットをフレームバッファに戻す。
	g_graphicsEngine->ChangeRenderTarget(
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//ドロドロ
	m_copyMainRtToFrameBufferSprite.Draww();

	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
	g_graphicsEngine->EndRender();
}

void GameObjectManager::UpdateSceneGraph()
{
	//ワールド行列を更新。
	for (auto transform : m_childrenOfRootTransformList) {
		transform->UpdateWorldMatrixAll();
	}
}
void GameObjectManager::ExecuteDeleteGameObjects()
{
	int preBufferNo = m_currentDeleteObjectBufferNo;
	//バッファを切り替え。
	m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
	for (GameObjectList& goList : m_deleteObjectArray[preBufferNo]) {
		for (GameObject* go : goList) {
			GameObjectPrio prio = go->GetPriority();
			GameObjectList& goExecList = m_gameObjectListArray.at(prio);
			auto it = std::find(goExecList.begin(), goExecList.end(), go);
			if (it != goExecList.end()) {
				//削除リストから除外された。
				(*it)->m_isRegistDeadList = false;
				if ((*it)->IsNewFromGameObjectManager()) {
					delete (*it);
				}
			}
			goExecList.erase(it);
		}
		goList.clear();
	}
}
void GameObjectManager::Init(int gameObjectPrioMax)
{
	assert(gameObjectPrioMax <= GAME_OBJECT_PRIO_MAX && "ゲームオブジェクトの優先度の最大数が大きすぎます。");
	m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
	m_gameObjectListArray.resize(gameObjectPrioMax);
	m_deleteObjectArray[0].resize(gameObjectPrioMax);
	m_deleteObjectArray[1].resize(gameObjectPrioMax);
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	//メインレンダリングターゲットに描かれた絵を
	//フレームバッファにコピーするためのスプライトを初期化する。
	m_copyMainRtToFrameBufferSprite.Init(
		m_mainRenderTarget.GetRenderTargetSRV(),
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
}