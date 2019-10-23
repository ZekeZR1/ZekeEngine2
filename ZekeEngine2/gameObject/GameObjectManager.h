#ifndef _CGAMEOBJECTMANAGER_H_
#define _CGAMEOBJECTMANAGER_H_

#include "GameObject.h"
#include "..//graphics/CUtil.h"
#include "CTransform.h"
#include "../graphics/ShadowMap.h"
#include "../graphics/postEffect/Bloom.h"
#include "../graphics/postEffect/PostEffect.h"

class CTransform;

class GameObjectManager : Noncopyable
{
private:
	GameObjectManager() :
		m_gameObjectPriorityMax(0)
	{
	}
	~GameObjectManager() {
		m_mainRenderTarget.ReleaseRenderTarget();
		if (m_frameBufferRenderTargetView != nullptr) {
			m_frameBufferRenderTargetView->Release();
		}
		if (m_frameBufferDepthStencilView != nullptr) {
			m_frameBufferDepthStencilView->Release();
		}
		m_shadowMap.ClearShadowCasters();
	}
	/*!
		*@brief	ゲームオブジェクトの名前キーを作成。
		*/
	static unsigned int MakeGameObjectNameKey(const char* objectName)
	{
		static const unsigned int defaultNameKey = CUtil::MakeHash("Undefined");	//名前キー。
		unsigned int hash;
		if (objectName == nullptr) {
			hash = defaultNameKey;
		}
		else {
			hash = CUtil::MakeHash(objectName);
		}
		return hash;
	}
public:
	void AddShadowCaster(SkinModel* model) {
		m_shadowMap.RegistShadowCaster(model);
	}
	void ClearShadowCasters() {
		m_shadowMap.ClearShadowCasters();
	}

	void ClearACaster(SkinModel* casterModel) {
		m_shadowMap.ClearACaster(casterModel);
	}
	/*!
 *@brief	インスタンスの取得。
 */
	static GameObjectManager& Instance()
	{
		static GameObjectManager instance;
		return instance;
	}
	/*!
	*@brief	実行。
	*/
	void Execute();
	/*!
	 *@brief	初期化。
	 *@param[in]	gameObjectPrioMax	ゲームオブジェクトの優先度の最大値。(255まで)
	 */
	void Init(int gameObjectPrioMax);
	/*!
	*@brief	ゲームオブジェクトの追加。
	*@param[in]	prio			実行優先順位。
	*@param[in] go				ゲームオブジェクト
	*@param[in] objectName		オブジェクト名
	*/
	void AddGameObject(GameObjectPrio prio, GameObject* go, const char* objectName = nullptr)
	{
		(void)objectName;
		if (!go->m_isRegist) {
			go->Awake();
			unsigned int hash = MakeGameObjectNameKey(objectName);
			m_gameObjectListArray.at(prio).push_back(go);
			go->m_isRegist = true;
			go->m_priority = prio;
			go->m_isStart = false;
			go->m_nameKey = hash;
			if (go->m_isDead) {
				//死亡フラグが立っている。
				//削除リストに入っていたらそこから除去する。
				go->m_isDead = false;
			}

		}
	}
	/*!
	 *@brief	ゲームオブジェクトのnew
	 *@details
	 * この関数を使用してnewしたオブジェクトは必ずDeleteGameObjectを実行することでdeleteされます。
	 *@param[in]	prio		実行優先順位。
	 *@param[in]	objectName	オブジェクト名。
	 *@param[in]	ctorArgs	コンストラクタに渡す可変長引数。
	 */
	template<class T, class... TArgs>
	T* NewGameObject(GameObjectPrio prio, const char* objectName, TArgs... ctorArgs)
	{
		(void*)objectName;
		assert(prio <= m_gameObjectPriorityMax && "ゲームオブジェクトの優先度の最大数が大きすぎます。");
		T* newObject = new T(ctorArgs...);
		newObject->Awake();
		newObject->SetMarkNewFromGameObjectManager();
		m_gameObjectListArray.at(prio).push_back(newObject);
		unsigned int hash = MakeGameObjectNameKey(objectName);
		newObject->m_isRegist = true;
		newObject->m_priority = prio;
		newObject->m_nameKey = hash;
		return newObject;
	}
	/*!
	 *@brief	ゲームオブジェクトの削除。
	 */
	void DeleteGameObject(GameObject* gameObject)
	{
		if (gameObject != nullptr
			&& gameObject->m_isRegist
			) {
			gameObject->SetDeadMark();
			gameObject->OnDestroy();
			gameObject->m_isRegist = false;
			gameObject->m_isRegistDeadList = true;

			m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
			gameObject = nullptr;
		}
	}
	/*!
	*@brief	ゲームオブジェクトの検索。
	*@details
	* 重いよ！
	*@param[in]	objectName		オブジェクト名。
	*/
	template<class T>
	T* FindGameObject(const char* objectName)
	{
		unsigned int nameKey = CUtil::MakeHash(objectName);
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr) {
						//型変換に失敗。
					}
					return p;
				}
			}
		}
		//見つからなかった。
		return nullptr;
	}
	template<class T>
	void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = CUtil::MakeHash(objectName);
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
			}
		}
	}
	/*!
	*@brief	指定したタグのいずれかがが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
	*/

	void FindGameObjectsWithTag(unsigned int tags, std::function<void(GameObject* go)>func)
	{
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				unsigned int goTags = go->GetTags();
				if ((goTags & tags) != 0) {
					func(go);
				}
			}
		}

	}
	ShadowMap* GetShadowMap() {
		return &m_shadowMap;
	}
private:
	/*!
		 *@brief	ゲームオブジェクトの削除を実行。
		 */
	void ExecuteDeleteGameObjects();
	/*!
	*@brief	シーングラフの更新。
	*/
	void UpdateSceneGraph();
	public:
	RenderTarget* GetMainRenderTarget() {
		return &m_mainRenderTarget;
	}

	void SetShadowCameraPosAndTarget(CVector3 pos, CVector3 target) {
		m_lightCameraPos = pos;
		m_lightCameraTarget = target;
	}

private:
	CVector3 m_lightCameraPos = { 0,1,0 };
	CVector3 m_lightCameraTarget = { 0,0,0 };
	ShadowMap m_shadowMap;					//シャドウマップ。
	RenderTarget m_mainRenderTarget;		//メインレンダリングターゲット。
	PostEffect m_postEffect;				//ポストエフェクト。
	Sprite m_copyMainRtToFrameBufferSprite;			//メインレンダリングターゲットに描かれた絵をフレームバッファにコピーするためのスプライト。
	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。
	//std::vector<SkinModel*> m_shadowCasters;
	SkinModel* m_shadowCaster = nullptr;
	CTransform m_transform;												//!<Transform。
	typedef std::list<GameObject*>	GameObjectList;
	std::vector<GameObjectList>	m_gameObjectListArray;					//!<ゲームオブジェクトの優先度付きリスト。
	std::vector<GameObjectList>	m_deleteObjectArray[2];					//!<削除するオブジェクトのリスト。削除処理を行っている最中にDeleteGameObjectが呼ばれる可能性が高いので、ダブルバッファ化。
	std::list<CTransform*>		m_childrenOfRootTransformList;			//!<ルートの子供のTransformのリスト。
	GameObjectPrio				m_gameObjectPriorityMax;				//!<ゲームオブジェクトの優先度の最大数。
	int m_currentDeleteObjectBufferNo = 0;								//!<現在の削除オブジェクトのバッファ番号。
	static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;
};


static inline GameObjectManager& IGameObjectManager()
{
	return GameObjectManager::Instance();
}

/*!
 *@brief	ゲームオブジェクト生成のヘルパー関数。
 *@param[in]	priority	プライオリティ。
 *@param[in]	objectName	オブジェクト名。(NULLの指定可）
 *@param[in]	ctorArgs	コンストラクタに渡す可変長引数。
 */
template<class T, class... TArgs>
static inline T* NewGO(int priority, const char* objectName = nullptr, TArgs... ctorArgs)
{
	return IGameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objectName, ctorArgs...);
}


/*!
	 *@brief	ゲームオブジェクト削除のヘルパー関数。
	 * NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
	 *@param[in]	go		削除するゲームオブジェクト。
	 */
static inline void DeleteGO(GameObject* go)
{
	IGameObjectManager().DeleteGameObject(go);
}

/*!
	 *@brief	ゲームオブジェクトの追加のヘルパー関数。
	 *@param[in]	go			追加するゲームオブジェクト。
	 *@param[in]	priority	プライオリティ。
	 *@param[in]	objectName	ゲームオブジェクトの名前。
	 */
static inline void AddGO(int priority, GameObject* go, const char* objectName = nullptr)
{
	IGameObjectManager().AddGameObject(static_cast<GameObjectPrio>(priority), go, objectName);
}
/*!
*@brief	指定したタグのいずれかがが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
*/
static inline 	void FindGameObjectsWithTag(unsigned int tags, std::function<void(GameObject* go)>func)
{
	IGameObjectManager().FindGameObjectsWithTag(tags, func);
}
/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@param[in]	objectName	ゲームオブジェクトの名前。
*/
template<class T>
static inline T* FindGO(const char* objectName)
{
	return IGameObjectManager().FindGameObject<T>(objectName);
}
/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@details
* 同名のゲームオブジェクトに全てに対して、クエリを行いたい場合に使用してください。
*@param[in]	objectName	ゲームオブジェクトの名前。
*@param[in]	func		ゲームオブジェクトが見つかったときに呼ばれるコールバック関数。
*/
template<class T>
static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
{
	return IGameObjectManager().FindGameObjects<T>(objectName, func);
}

#endif // _CGAMEOBJECTMANAGER_H
