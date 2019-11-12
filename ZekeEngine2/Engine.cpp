#include "stdafx.h"
#include "Engine.h"
#include "FPSCounter.h"

CFPSCounter* FPS = nullptr;

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int nWheelFraction = 0;	// 回転量の端数

	switch (msg)
	{
	case WM_ACTIVATEAPP:
		//DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		//ZKeyBoard().GetKeyboard()->ProcessMessage(msg, wParam, lParam);
		ZKeyBoard().GetKeyboard()->ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		//DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		ZKeyBoard().GetKeyboard()->ProcessMessage(msg, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		// ウィンドウのアクティブ切り替え時に端数をリセット
		nWheelFraction = 0;
		break;

	case WM_MOUSEWHEEL:
	{

		DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);	// 同時に押されているキー情報
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// 回転量

		// 前回の端数を追加
		// ノッチ数を求める
		int nNotch = zDelta / WHEEL_DELTA;
		Engine().SetMouseNotch(nNotch);
		// 端数を保存する
		nWheelFraction = zDelta % WHEEL_DELTA;

		break;
	}
	case WM_LBUTTONDOWN:
	{
		//左クリックされた
		Engine().SetMouseEvent(enLeftClick, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		//左ボタンを離した
		Engine().SetMouseEvent(enLeftClick, false);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		//ミドルクリックされた
		Engine().SetMouseEvent(enMiddleClick, true);
		break;
	}
	case WM_MBUTTONUP:
	{
		//ミドルボタンを離した
		Engine().SetMouseEvent(enMiddleClick, false);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		//右クリックされた
		Engine().SetMouseEvent(enRightClick, true);
		break;
	}
	case WM_RBUTTONUP:
	{
		//右ボタンを離した
		Engine().SetMouseEvent(enRightClick, false);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CEngine::Init(const EngineParam& engineParam) {
	if (!InitWindow(engineParam)) {
		return false;
	}
	if (!m_graphicsEngine.InitDirectX(m_hWnd)) {
		return false;
	}
	m_physicsWorld.Init();
	IGameObjectManager().Init(32);
	//TODO :各エンジン初期化
	// : サウンドエンジン初期化
	// : エフェクトエンジン初期化
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
	ZKeyBoard().Update();
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
