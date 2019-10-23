#include "stdafx.h"
#include "Mouse.h"

namespace Mouse {
	//TODO :　マウス変数管理するクラスを作成する
	bool trigger[enNumMouseEve] = { false };
	bool prePress[enNumMouseEve] = { false };
	bool press[enNumMouseEve] = { false };
	CVector3 cursorpos = CVector3::Zero();
	int Notch = 0;

	void UpdateMouseInput() {
		auto hwnd = Engine().GetWindowHandle();
		POINT pt;
		GetCursorPos(&pt);

		POINT fl;
		fl.x = 0;
		fl.y = 0;
		ClientToScreen(hwnd, &fl);

		RECT crect;
		GetClientRect(hwnd, &crect);

		float zrx = fl.x + crect.right / 2.f;
		float zry = fl.y + crect.bottom / 2.f;

		cursorpos.x = (float)pt.x - zrx;
		cursorpos.x *= 1280.f / crect.right;

		cursorpos.y = zry - (float)pt.y;
		cursorpos.y *= 720.f / crect.bottom;

		//カーソル表示
		ShowCursor(FALSE);
#if _DEBUG
		ShowCursor(TRUE);
#endif
		//マウスノッチ
		{
			if (Engine().GetMouseNotch() > 0) {
				trigger[enNotchUp] = 1 ^ press[enNotchUp];
				press[enNotchUp] = 1;
			}
			else {
				trigger[enNotchUp] = 0;
				press[enNotchUp] = 0;
			}
			if (Engine().GetMouseNotch() < 0) {
				trigger[enNotchDown] = 1 ^ press[enNotchDown];
				press[enNotchDown] = 1;
			}
			else {
				trigger[enNotchDown] = 0;
				press[enNotchDown] = 0;
			}
		}
		//クリック
		{
			//左トリガー入力判定。
			if (Engine().GetMouseEvent(enLeftClick) != 0) {
				trigger[enLeftClick] = 1 ^ press[enLeftClick];
				press[enLeftClick] = 1;
			}
			else {
				trigger[enLeftClick] = 0;
				press[enLeftClick] = 0;
			}
			//右トリガー
			if (Engine().GetMouseEvent(enRightClick) != 0) {
				trigger[enRightClick] = 1 ^ press[enRightClick];
				press[enRightClick] = 1;
			}
			else {
				trigger[enRightClick] = 0;
				press[enRightClick] = 0;
			}
			//ミドルトリガー
			if (Engine().GetMouseEvent(enMiddleClick) != 0) {
				trigger[enMiddleClick] = 1 ^ press[enMiddleClick];
				press[enMiddleClick] = 1;
			}
			else {
				trigger[enMiddleClick] = 0;
				press[enMiddleClick] = 0;
			}
		}
		Notch = Engine().GetMouseNotch();
		Engine().SetMouseNotch(0);
	}
	CVector3 GetCursorPos() {
		return cursorpos;
	}
	int GetMouseNotch() {
		return Notch;
	}

	bool IsTrigger(enMouseEve eve) {
		return trigger[eve];
	}

	bool IsPress(enMouseEve eve) {
		return Engine().GetMouseEvent(eve);
	}
}