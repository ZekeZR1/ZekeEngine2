/*!
*@brief	パッド。
*/
#include "stdafx.h"
#include "Pad.h"
#include <limits.h>

#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // 入力の遊びとなる範囲。

	namespace {
		/*!
		*@brief	仮想ボタンとXBoxコントローラのボタンとの関連付けを表す構造体。
		*/
		struct VirtualPadToXPad {
			EnButton vButton;		//!<仮想ボタン。
			DWORD	 xButton;		//!<XBoxコントローラのボタン。
		};

		const VirtualPadToXPad vPadToXPadTable[enButtonNum] = {
			{ enButtonUp		, XINPUT_GAMEPAD_DPAD_UP },
		{ enButtonDown		, XINPUT_GAMEPAD_DPAD_DOWN },
		{ enButtonLeft		, XINPUT_GAMEPAD_DPAD_LEFT },
		{ enButtonRight		, XINPUT_GAMEPAD_DPAD_RIGHT },
		{ enButtonA			, XINPUT_GAMEPAD_A },
		{ enButtonB			, XINPUT_GAMEPAD_B },
		{ enButtonY			, XINPUT_GAMEPAD_Y },
		{ enButtonX			, XINPUT_GAMEPAD_X },
		{ enButtonSelect	, XINPUT_GAMEPAD_BACK },
		{ enButtonStart		, XINPUT_GAMEPAD_START },
		{ enButtonRB1		, XINPUT_GAMEPAD_RIGHT_SHOULDER },
		{ enButtonRB2		, 0 },
		{ enButtonRB3		, XINPUT_GAMEPAD_RIGHT_THUMB },
		{ enButtonLB1		, XINPUT_GAMEPAD_LEFT_SHOULDER },
		{ enButtonLB2		, 0 },
		{ enButtonLB3		, XINPUT_GAMEPAD_LEFT_THUMB },
		{ enGoForward,		0 },
		{ enGoBack,		0 },
		//{enTurnRight ,XINPUT_GAMEPAD_}
		};
		/*!
		*@brief	仮想ボタンとキーボードとの関連付けを表す構造体。
		*/
		struct VirtualPadToKeyboard {
			EnButton vButton;		//!<仮想ボタン
			DWORD keyCoord;			//!<キーボードのキーコード。
		};
		const VirtualPadToKeyboard vPadToKeyboardTable[enButtonNum] = {
			{ enButtonUp		, '8' },
		{ enButtonDown		, '2' },
		{ enButtonLeft		, '4' },
		{ enButtonRight		, '6' },
		{ enButtonA			, 'J' },
		{ enButtonB			, 'K' },
		{ enButtonY			, 'I' },
		{ enButtonX			, 'L' },
		{ enButtonSelect	, VK_SPACE },
		{ enButtonStart		, VK_RETURN },
		{ enButtonRB1		, '7' },
		{ enButtonRB2		, '8' },
		{ enButtonRB3		, '9' },
		{ enButtonLB1		, 'B' },
		{ enButtonLB2		, 'N' },
		{ enButtonLB3		, 'M' },
		//action
		{ enGoForward		, 'W' },
		{ enGoBack			, 'S' },
		{ enTurnRight			, 'D' },
		{ enTurnLeft			, 'A' },
		{ enJump			, RI_MOUSE_RIGHT_BUTTON_DOWN },
		{ enBoost			, RI_MOUSE_LEFT_BUTTON_DOWN },
		{ enDrift			, VK_SHIFT },
		{ enAirTrunRight			, 'E' },
		{ enAirTurnLeft			, 'Q' },
		{ enAirPitchUp			, 'S' },
		{ enAirPitchDown			, 'W' },
		{ enAirRoll			, VK_CONTROL },
		};
	}
	CPad::CPad()
	{
		memset(&m_state, 0, sizeof(m_state));
		memset(m_trigger, 0, sizeof(m_trigger));
		memset(m_press, 0, sizeof(m_press));
	}
	CPad::~CPad()
	{
	}
	void CPad::Update()
	{
		DWORD result = XInputGetState(m_padNo, &m_state.state);
		if (result == ERROR_SUCCESS) {
			//接続されている。
			m_state.bConnected = true;
			for (const VirtualPadToXPad& vPadToXPad : vPadToXPadTable) {
				if (m_state.state.Gamepad.wButtons & vPadToXPad.xButton) {
					m_trigger[vPadToXPad.vButton] = 1 ^ m_press[vPadToXPad.vButton];
					m_press[vPadToXPad.vButton] = 1;
				}
				else {
					m_trigger[vPadToXPad.vButton] = 0;
					m_press[vPadToXPad.vButton] = 0;
				}
			}
			//左トリガー。
			if (m_state.state.Gamepad.bLeftTrigger != 0) {
				m_trigger[enButtonLB2] = 1 ^ m_press[enButtonLB2];
				m_press[enButtonLB2] = 1;
			}
			else {
				m_trigger[enButtonLB2] = 0;
				m_press[enButtonLB2] = 0;
			}
			//go back
			//左トリガー。
			if (m_state.state.Gamepad.bLeftTrigger != 0) {
				m_trigger[enGoBack] = 1 ^ m_press[enGoBack];
				m_press[enGoBack] = 1;
			}
			else {
				m_trigger[enGoBack] = 0;
				m_press[enGoBack] = 0;
			}
			//右トリガー
			if (m_state.state.Gamepad.bRightTrigger != 0) {
				m_trigger[enButtonRB2] = 1 ^ m_press[enButtonRB2];
				m_press[enButtonRB2] = 1;
			}
			else {
				m_trigger[enButtonRB2] = 0;
				m_press[enButtonRB2] = 0;
			}
			//go forward
			if (m_state.state.Gamepad.bRightTrigger != 0) {
				m_trigger[enGoForward] = 1 ^ m_press[enGoForward];
				m_press[enGoForward] = 1;
			}
			else {
				m_trigger[enGoForward] = 0;
				m_press[enGoForward] = 0;
			}
			//trigger
			if ((m_state.state.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				m_state.state.Gamepad.bLeftTrigger = 0;
				m_lTrigger = 0.0f;
			}
			else {
				//Left Trigger
				if (m_state.state.Gamepad.bLeftTrigger > 0) {
					m_lTrigger = static_cast<float>(m_state.state.Gamepad.bLeftTrigger) / 255;
				}
			}

			if ((m_state.state.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				m_state.state.Gamepad.bRightTrigger = 0;
				m_rTrigger = 0.0f;
			}
			else {
				//Right Trigger
				if (m_state.state.Gamepad.bRightTrigger > 0) {
					m_rTrigger = static_cast<float>(m_state.state.Gamepad.bRightTrigger) / 255;
				}
			}
			//
			if ((m_state.state.Gamepad.sThumbLX < INPUT_DEADZONE &&
				m_state.state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
				(m_state.state.Gamepad.sThumbLY < INPUT_DEADZONE &&
					m_state.state.Gamepad.sThumbLY > -INPUT_DEADZONE))
			{
				m_state.state.Gamepad.sThumbLX = 0;
				m_state.state.Gamepad.sThumbLY = 0;
				m_lStickX = 0.0f;
				m_lStickY = 0.0f;
			}
			else {
				//左スティックの入力量。
				if (m_state.state.Gamepad.sThumbLX > 0) {
					m_lStickX = static_cast<float>(m_state.state.Gamepad.sThumbLX) / SHRT_MAX;
				}
				else {
					m_lStickX = static_cast<float>(m_state.state.Gamepad.sThumbLX) / -SHRT_MIN;
				}
				if (m_state.state.Gamepad.sThumbLY > 0) {
					m_lStickY = static_cast<float>(m_state.state.Gamepad.sThumbLY) / SHRT_MAX;
				}
				else {
					m_lStickY = static_cast<float>(m_state.state.Gamepad.sThumbLY) / -SHRT_MIN;
				}
			}

			if ((m_state.state.Gamepad.sThumbRX < INPUT_DEADZONE &&
				m_state.state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
				(m_state.state.Gamepad.sThumbRY < INPUT_DEADZONE &&
					m_state.state.Gamepad.sThumbRY > -INPUT_DEADZONE))
			{
				m_state.state.Gamepad.sThumbRX = 0;
				m_state.state.Gamepad.sThumbRY = 0;
				m_rStickX = 0.0f;
				m_rStickY = 0.0f;
			}
			else {
				//右スティックの入力量。
				if (m_state.state.Gamepad.sThumbRX > 0) {
					m_rStickX = static_cast<float>(m_state.state.Gamepad.sThumbRX) / SHRT_MAX;
				}
				else {
					m_rStickX = static_cast<float>(m_state.state.Gamepad.sThumbRX) / -SHRT_MIN;
				}
				if (m_state.state.Gamepad.sThumbRY > 0) {
					m_rStickY = static_cast<float>(m_state.state.Gamepad.sThumbRY) / SHRT_MAX;
				}
				else {
					m_rStickY = static_cast<float>(m_state.state.Gamepad.sThumbRY) / -SHRT_MIN;
				}
			}
		}
		else {
			//接続されていない場合はキーボードの入力でエミュレートする。
			if (m_state.bConnected) {
				//未接続になった。
				memset(&m_state, 0, sizeof(m_state));
				memset(m_trigger, 0, sizeof(m_trigger));
				memset(m_press, 0, sizeof(m_press));
			}
			m_lStickX = 0.0f;
			m_lStickY = 0.0f;
			m_rStickX = 0.0f;
			m_rStickY = 0.0f;

			if (GetAsyncKeyState(VK_LEFT)) {
				m_rStickX = -1.0f;
			}
			else if (GetAsyncKeyState(VK_RIGHT)) {
				m_rStickX = 1.0f;
			}
			if (GetAsyncKeyState(VK_UP)) {
				m_rStickY = 1.0f;
			}
			else if (GetAsyncKeyState(VK_DOWN)) {
				m_rStickY = -1.0f;
			}
			//スティックの入力量を正規化。
			float t = fabsf(m_rStickX) + fabsf(m_rStickY);
			if (t > 0.0f) {
				m_rStickX /= t;
				m_rStickY /= t;
			}

			if (GetAsyncKeyState('A')) {
				m_lStickX = -1.0f;
			}
			else if (GetAsyncKeyState('D')) {
				m_lStickX = 1.0f;
			}
			if (GetAsyncKeyState('W')) {
				m_lStickY = 1.0f;
			}
			else if (GetAsyncKeyState('S')) {
				m_lStickY = -1.0f;
			}
			//スティックの入力量を正規化。
			t = fabsf(m_lStickX) + fabsf(m_lStickY);
			if (t > 0.0f) {
				m_lStickX /= t;
				m_lStickY /= t;
			}

			for (const VirtualPadToKeyboard& vPadToKeyboard : vPadToKeyboardTable) {
				if (GetAsyncKeyState(vPadToKeyboard.keyCoord)) {
					m_trigger[vPadToKeyboard.vButton] = 1 ^ m_press[vPadToKeyboard.vButton];
					m_press[vPadToKeyboard.vButton] = 1;
				}
				else {
					m_trigger[vPadToKeyboard.vButton] = 0;
					m_press[vPadToKeyboard.vButton] = 0;
				}
			}
		}
	}

