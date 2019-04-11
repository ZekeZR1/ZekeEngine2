/*!
 *@brief	�Q�[���I�u�W�F�N�g�̊��N���X�B
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
	 *@brief	�C���X�^���X���������ꂽ����ɌĂ΂��֐��B
	 *@details	�R���X�g���N�^���Ă΂ꂽ��ő��Ă΂�܂��B
	 */
	virtual void Awake() {}
	/*!
	 *@brief	Update�֐������s�����O�ɌĂ΂��X�V�֐��B
	 */
	virtual void PreUpdate() {}
	/*!
	 *@brief	Update�֐������s���ꂽ��ŌĂ΂��X�V�֐��B
	 */
	virtual void PostUpdate() {}
	/*!
	 *@brief	Render�֐������s�����O�ɌĂ΂��`�揈���B
	 */
	virtual void PreRender(CRenderContext& renderContext) {
		(void)renderContext;
	}
	virtual void PreRender() {
	}
	/*!
	 *@brief	Render�֐������s���ꂽ��ŌĂ΂��`�揈��
	 *@details
	 * �|�X�g�G�t�F�N�g�̌�Ŏ��s����܂��BHUD�Ȃǃ|�X�g�G�t�F�N�g�̉e�����󂯂����Ȃ��`�敨�͂����Ń����_�����O���Ă��������B
	 */
	//virtual void PostRender(CRenderContext& renderContext) {
	virtual void PostRender() {
		//(void)renderContext;
	}
	/*!
	*@brief	���S�t���O�𗧂Ă�B
	*@details
	* ���̊֐��̓G���W���̊O����͎��s���Ȃ��ł��������B
	*/
	void SetDeadMark()
	{
		m_isDead = true;
	}
	/*!
	*@brief	���S����B
	*/
	bool IsDead() const
	{
		return m_isDead;
	}
	/*!
	*@brief Start�֐������������H
	*/
	virtual bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief �A�N�e�B�u���ǂ����𔻒�B
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/*!
	*@brief �A�N�e�B�u�t���O��ݒ�B
	*/
	void SetActiveFlag(bool flag)
	{
		m_isActive = flag;
	}
	/*!
	*@brief �^�O��ݒ�B
	*/
	void SetTags(unsigned int tags)
	{
		m_tags = tags;
	}
	/*!
	*@brief �^�O���擾�B
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
				//���������������B
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
	GameObjectPrio	m_priority;			//!<���s�D��x�B
	bool m_isStart;						//!<Start�̊J�n�t���O�B
	bool m_isDead;						//!<���S�t���O�B
	bool m_isRegistDeadList = false;	//!<���S���X�g�ɐς܂�Ă���B
	bool m_isNewFromGameObjectManager;	//!<GameObjectManager��new���ꂽ�B
	bool m_isRegist = false;			//!<GameObjectManager�ɓo�^����Ă���H
	bool m_isActive = true;				//!<Active�t���O�B
	unsigned int m_tags = 0;			//!<�^�O�B
	unsigned int m_nameKey = 0;			//!<���O�L�[�B

};

#endif // _CGAMEOBJECT_H_