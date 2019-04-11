/*!
 *@brief	ゲームオブジェクトの基底クラス。
 */

#pragma once
#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_

class GameObjectManager;
typedef unsigned char	 GameObjectPrio;

class GameObject : Noncopyable
{
public:
	GameObject() :
		m_priority(0),
		m_isStart(false),
		m_isDead(false),
		m_isNewFromGameObjectManager(false) 
	{
	}
	virtual ~GameObject() {};
public:
	virtual bool Start() { return true; }
	virtual void Update() {}
	//virtual void Render(CRenderContext& renderContext)
	virtual void Render()
	{
		//(void)renderContext;
	}
	virtual void OnDestroy(){}

	GameObjectPrio GetPriority() const
	{
		return m_priority;
	}

public:
	/*!
	 *@brief	インスタンスが生成された直後に呼ばれる関数。
	 *@details	コンストラクタが呼ばれた後で即呼ばれます。
	 */
	virtual void Awake() {}
	/*!
	 *@brief	Update関数が実行される前に呼ばれる更新関数。
	 */
	virtual void PreUpdate() {}
	/*!
	 *@brief	Update関数が実行された後で呼ばれる更新関数。
	 */
	virtual void PostUpdate() {}
	/*!
	 *@brief	Render関数が実行される前に呼ばれる描画処理。
	 */
	virtual void PreRender(CRenderContext& renderContext) {
		(void)renderContext;
	}
	virtual void PreRender() {
	}
	/*!
	 *@brief	Render関数が実行された後で呼ばれる描画処理
	 *@details
	 * ポストエフェクトの後で実行されます。HUDなどポストエフェクトの影響を受けたくない描画物はここでレンダリングしてください。
	 */
	//virtual void PostRender(CRenderContext& renderContext) {
	virtual void PostRender() {
		//(void)renderContext;
	}
	/*!
	*@brief	死亡フラグを立てる。
	*@details
	* この関数はエンジンの外からは実行しないでください。
	*/
	void SetDeadMark()
	{
		m_isDead = true;
	}
	/*!
	*@brief	死亡判定。
	*/
	bool IsDead() const
	{
		return m_isDead;
	}
	/*!
	*@brief Start関数が完了した？
	*/
	virtual bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief アクティブかどうかを判定。
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/*!
	*@brief アクティブフラグを設定。
	*/
	void SetActiveFlag(bool flag)
	{
		m_isActive = flag;
	}
	/*!
	*@brief タグを設定。
	*/
	void SetTags(unsigned int tags)
	{
		m_tags = tags;
	}
	/*!
	*@brief タグを取得。
	*/
	unsigned int GetTags() const
	{
		return m_tags;
	}

	//void PostRenderWrapper(CRenderContext& renderContext)
	void PostRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PostRender();
		}
	}
	//void RenderWrapper(CRenderContext& renderContext)
	void RenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			Render();
		}
	}
	//void PreRenderWrapper(CRenderContext& renderContext)
	void PreRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PreRender();
		}
	}
	void PostUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PostUpdate();
		}
	}
	void PreUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PreUpdate();
		}
	}
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			Update();
		}
	}
	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead && !m_isRegistDeadList) {
			if (Start()) {
				//初期化処理完了。
				m_isStart = true;
			}
		}
	}
	void SetMarkNewFromGameObjectManager()
	{
		m_isNewFromGameObjectManager = true;
	}
	bool IsNewFromGameObjectManager() const
	{
		return m_isNewFromGameObjectManager;
	}
	friend class GameObjectManager;
protected:
	GameObjectPrio	m_priority;			//!<実行優先度。
	bool m_isStart;						//!<Startの開始フラグ。
	bool m_isDead;						//!<死亡フラグ。
	bool m_isRegistDeadList = false;	//!<死亡リストに積まれている。
	bool m_isNewFromGameObjectManager;	//!<GameObjectManagerでnewされた。
	bool m_isRegist = false;			//!<GameObjectManagerに登録されている？
	bool m_isActive = true;				//!<Activeフラグ。
	unsigned int m_tags = 0;			//!<タグ。
	unsigned int m_nameKey = 0;			//!<名前キー。

};

#endif // _CGAMEOBJECT_H_