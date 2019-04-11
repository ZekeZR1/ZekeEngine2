#include "stdafx.h"

HWND g_hwnd = NULL;
GraphicsEngine* g_graphicsEngine = NULL;
int notch = 0;
bool mEve[10];
int g_Key = 0;
bool g_isPressShift = false;
//TODO : フルスクリーン表示できるようにする
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int nWheelFraction = 0;	// 回転量の端数
	
	switch (msg)
	{
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
		zDelta += nWheelFraction;
		// ノッチ数を求める
		int nNotch = zDelta / WHEEL_DELTA;
		notch = nNotch;
		// 端数を保存する
		nWheelFraction = zDelta % WHEEL_DELTA;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		//左クリックされた
		mEve[0] = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		//左ボタンを離した
		mEve[0] = false;
		break;
	}
	case WM_MBUTTONDOWN:
	{
		//ミドルクリックされた
		mEve[1] = true;
		break;
	}
	case WM_MBUTTONUP:
	{
		//ミドルボタンを離した
		mEve[1] = false;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		//右クリックされた
		mEve[2] = true;
		break;
	}
	case WM_RBUTTONUP:
	{
		//右ボタンを離した
		mEve[2] = false;
		break;
	}
	case WM_KEYDOWN:
	{
		
		g_Key = wParam;
		if (wParam == VK_SHIFT)
		{
			g_isPressShift = true;
		}
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
		
		if (wParam == VK_SHIFT)
		{
			g_isPressShift = false;
		}
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
	//return 0;
}

void InitWindow(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow,
	const TCHAR* appName){
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		MsgProc,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		appName,
		NULL
	};

	RegisterClassEx(&wc);

	g_hwnd = CreateWindow(
		appName,
		appName,
		WS_OVERLAPPEDWINDOW,	
		0,					
		0,					
		1280,					
		720,					
		NULL,	
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(g_hwnd, nCmdShow);

	//DirectX
	g_graphicsEngine = new GraphicsEngine();
	g_graphicsEngine->InitDirectX(g_hwnd);
	//Bullet
	g_physics.Init();
	//Pad
	g_pad[0].Init(0);
	g_pad[1].Init(1);
	g_pad[2].Init(2);
	g_pad[3].Init(3);
}


bool DispatchWindowMessage() {
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			break;
		}
	}
	return msg.message != WM_QUIT;
}
