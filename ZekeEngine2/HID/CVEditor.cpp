#include "stdafx.h"
#include "CVEditor.h"

bool CVEditor::Start() {
	return true;
}

void CVEditor::OnDestroy() {
	DeleteGO(m_font);
}

void CVEditor::CreateText(int priority) {
	m_font = NewGO<FontRender>(priority);
	m_font->Init(L"", m_fontInfo.pos, m_fontInfo.rot, m_fontInfo.col, m_fontInfo.scale, m_fontInfo.pivot);
}

void CVEditor::Update() {
	if (!m_isEditorActive) return;

	if (ZKeyBoard().GetStateTracker().pressed.Back){
		if(m_str.length() > 0)
			m_str.erase(m_str.length() - 1);
	}

	if (m_str.length() > m_numMaxChar) return;

	if (ZKeyBoard().GetStateTracker().pressed.A)
		m_str += L"a";
	if (ZKeyBoard().GetStateTracker().pressed.B)
		m_str += L"b";
	if (ZKeyBoard().GetStateTracker().pressed.C)
		m_str += L"c";
	if (ZKeyBoard().GetStateTracker().pressed.D)
		m_str += L"d";
	if (ZKeyBoard().GetStateTracker().pressed.E)
		m_str += L"e";
	if (ZKeyBoard().GetStateTracker().pressed.F)
		m_str += L"f";
	if (ZKeyBoard().GetStateTracker().pressed.G)
		m_str += L"g";
	if (ZKeyBoard().GetStateTracker().pressed.H)
		m_str += L"h";
	if (ZKeyBoard().GetStateTracker().pressed.I)
		m_str += L"i";
	if (ZKeyBoard().GetStateTracker().pressed.J)
		m_str += L"j";
	if (ZKeyBoard().GetStateTracker().pressed.K)
		m_str += L"k";
	if (ZKeyBoard().GetStateTracker().pressed.L)
		m_str += L"l";
	if (ZKeyBoard().GetStateTracker().pressed.M)
		m_str += L"m";
	if (ZKeyBoard().GetStateTracker().pressed.N)
		m_str += L"n";
	if (ZKeyBoard().GetStateTracker().pressed.O)
		m_str += L"o";
	if (ZKeyBoard().GetStateTracker().pressed.P)
		m_str += L"p";
	if (ZKeyBoard().GetStateTracker().pressed.Q)
		m_str += L"q";
	if (ZKeyBoard().GetStateTracker().pressed.R)
		m_str += L"r";
	if (ZKeyBoard().GetStateTracker().pressed.S)
		m_str += L"s";
	if (ZKeyBoard().GetStateTracker().pressed.T)
		m_str += L"t";
	if (ZKeyBoard().GetStateTracker().pressed.U)
		m_str += L"u";
	if (ZKeyBoard().GetStateTracker().pressed.V)
		m_str += L"v";
	if (ZKeyBoard().GetStateTracker().pressed.W)
		m_str += L"w";
	if (ZKeyBoard().GetStateTracker().pressed.X)
		m_str += L"x";
	if (ZKeyBoard().GetStateTracker().pressed.Y)
		m_str += L"y";
	if (ZKeyBoard().GetStateTracker().pressed.Z)
		m_str += L"z";

	m_font->SetText(m_str);

}