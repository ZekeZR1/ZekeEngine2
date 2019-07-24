#include "stdafx.h"
#include "Engine.h"
#include "FPSCounter.h"

CFPSCounter* FPS = nullptr;

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int nWheelFraction = 0;	// ��]�ʂ̒[��

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		// �E�B���h�E�̃A�N�e�B�u�؂�ւ����ɒ[�������Z�b�g
		nWheelFraction = 0;
		break;

	case WM_MOUSEWHEEL:
	{

		DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);	// �����ɉ�����Ă���L�[���
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// ��]��

		// �O��̒[����ǉ�
		//zDelta += nWheelFraction;
		// �m�b�`�������߂�
		int nNotch = zDelta / WHEEL_DELTA;
		Engine().SetMouseNotch(nNotch);
		// �[����ۑ�����
		nWheelFraction = zDelta % WHEEL_DELTA;

		break;
	}
	case WM_LBUTTONDOWN:
	{
		//���N���b�N���ꂽ
		//mEve[0] = true;
		Engine().SetMouseEvent(enLeftClick, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		//���{�^���𗣂���
		//mEve[0] = false;
		Engine().SetMouseEvent(enLeftClick, false);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		//�~�h���N���b�N���ꂽ
		//mEve[1] = true;
		Engine().SetMouseEvent(enMiddleClick, true);
		break;
	}
	case WM_MBUTTONUP:
	{
		//�~�h���{�^���𗣂���
		//mEve[1] = false;
		Engine().SetMouseEvent(enMiddleClick, false);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		//�E�N���b�N���ꂽ
		//mEve[2] = true;
		Engine().SetMouseEvent(enRightClick, true);
		break;
	}
	case WM_RBUTTONUP:
	{
		//�E�{�^���𗣂���
		//mEve[2] = false;
		Engine().SetMouseEvent(enRightClick, false);
		break;
	}
	case WM_KEYDOWN:
	{

		Engine().SetKey(wParam);
		//g_Key = wParam;
		/*if (wParam == VK_SHIFT)
		{
			g_isPressShift = true;
		}*/
		/*switch (wParam)
		{
		case VK_RETURN:
			break;
		case VK_BACK:
			break;
		case VK_SHIFT:
			break;
		case VK_OEM_102:
			break;
		}*/
		break;
	}
	case WM_KEYUP:
	{

		/*if (wParam == VK_SHIFT)
		{
			g_isPressShift = false;
		}*/
		break;
	}
	default:
		break;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
	//return 0;
}

//LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, msg, wParam, lParam);
//	}
//
//	return 0;
//}
//void CEngine::Init(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPWSTR lpCmdLine,
//	int nCmdShow) {
//	//SetCurrentDirectory("Resource");
//	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, L"Game");
//	IGameObjectManager().Init(32);
//	FPS = new CFPSCounter(10);
//}

bool CEngine::Init(const EngineParam& engineParam) {
	if (!InitWindow(engineParam)) {
		return false;
	}
	if (!m_graphicsEngine.InitDirectX(m_hWnd)) {
		return false;
	}
	m_physicsWorld.Init();
	IGameObjectManager().Init(32);
	//TODO :�e�G���W��������
	// : �T�E���h�G���W��������
	// : �G�t�F�N�g�G���W��������
	// : ���̓f�o�C�X��������
	int padNo = 0;
	for (auto& pad : m_pad) {
		pad.Init(padNo);
		padNo++;
	}
	FPS = new CFPSCounter(10);
}

void CEngine::Release() {
	m_graphicsEngine.Release();
	m_physicsWorld.Release();
	delete FPS;
}

void CEngine::Update() {
	for (auto& pad : m_pad) {
		pad.Update();
	}
	Mouse::UpdateMouseInput();
	IGameObjectManager().Execute();
	m_physicsWorld.Update();
#if _DEBUG
	FPS->Draw();
#endif
}

void CEngine::GameRoop() {
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}
}

void CEngine::Final() {
	Release();
}

bool CEngine::InitWindow(const EngineParam& engineParam) {
	assert(engineParam.screenHeight != 0 && "screenHeight is zero");
	assert(engineParam.screenWidth != 0 && "screenWidth is zero");

	m_screenHeight = engineParam.screenHeight;
	m_screenWidth = engineParam.screenWidth;


	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
		TEXT("Game"), nullptr
	};
	RegisterClassEx(&wc);

	RECT rc = { 0, 0, m_screenWidth,m_screenHeight };
	AdjustWindowRect(
		&rc,
		WS_OVERLAPPED,
		FALSE
	);

	
	m_hWnd = CreateWindow(TEXT("Game"), TEXT("Game"),
		WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, wc.hInstance, nullptr);

	ShowWindow(m_hWnd, engineParam.nCmdShow);

	return m_hWnd != nullptr;
}
