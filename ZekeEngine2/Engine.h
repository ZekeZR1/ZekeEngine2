#pragma once

enum enMouseEve;

class CEngine : Noncopyable
{
public:
	bool Init(const EngineParam& engineParam);
	void Final();
	void GameRoop();

	static CEngine& GetInstance() {
		static CEngine instance;
		return instance;
	}

	CGraphicsEngine& GetGraphicsEngine()
	{
		return m_graphicsEngine;
	}

	CPhysicsWorld& GetPhysicsWorld() {
		return m_physicsWorld;
	}

	/*!
* @brief	�Q�[���p�b�h�̎擾�B
*@param[in]	padNo	�p�b�h�ԍ�
*/
	CPad& GetPad(int padNo)
	{
		//TK_ASSERT(padNo < CPad::CONNECT_PAD_MAX && padNo >= 0, "padNo is invalid");
		return m_pad[padNo];
	}
	HWND& GetWindowHandle() {
		return m_hWnd;
	}

	int GetMouseNotch() const {
		return m_notch;
	}
	void SetMouseNotch(int notch) {
		m_notch = notch;
	}
	bool IsPressShit() {
		return m_isPressShift;
	}
	void SetPressShift(bool f) {
		m_isPressShift = f;
	}
	bool GetMouseEvent(enMouseEve type) {
		return mEve[type];
	}
	void SetMouseEvent(enMouseEve type, bool flag) {
		mEve[type] = flag;
	}
	void SetKey(int f) {
		Key = f;
	}

	int GetKey() {
		return Key;
	}
private:
	//static Engine* instance;
	void Update();
	void Release();
	bool InitWindow(const EngineParam& engineParam);
	HINSTANCE				m_hinst = nullptr;					//�A�v���̃C���X�^���X
	HWND						m_hWnd = nullptr;					//�E�B���h�E�̃n���h��
	int							m_screenWidth = 0;				//�X�N���[���̕�
	int							m_screenHeight = 0;				//�X�N���[���̍���
	CGraphicsEngine		m_graphicsEngine;					//�O���t�B�b�N�G���W��
	CPhysicsWorld			m_physicsWorld;						//�������[���h
	CPad						m_pad[CPad::CONNECT_PAD_MAX];
	int m_notch = 0;
	bool mEve[10] = { false };
	int Key = 0;
	bool m_isPressShift = false;
};

static inline CEngine& Engine()
{
	return CEngine::GetInstance();
}

static inline CGraphicsEngine& GraphicsEngine() {
	return Engine().GetGraphicsEngine();
}

static inline Camera& MainCamera() {
	return GraphicsEngine().GetMainCamera();
}
static inline Camera& MainCamera2D()
{
	return GraphicsEngine().Get2DCamera();
}

static inline CPad& Pad(int padNo)
{
	return Engine().GetPad(padNo);
}

static inline CPhysicsWorld& PhysicsWorld() {
	return Engine().GetPhysicsWorld();
}


