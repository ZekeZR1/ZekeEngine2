#include "stdafx.h"
#include "CKeyboard.h"

CKeyboard::CKeyboard() {
	m_keyboard = std::make_unique<DirectX::Keyboard>();
}

void CKeyboard::Update() {
	m_tracker.Update(m_keyboard->GetState());
}