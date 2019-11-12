#include "stdafx.h"
#include "CVEditor.h"

bool CVEditor::Start() {
	m_font = NewGO<FontRender>(0);
	m_font->Init(L"", m_fontInfo.pos, m_fontInfo.rot, m_fontInfo.col, m_fontInfo.scale, m_fontInfo.pivot);
	return true;
}

void CVEditor::OnDestroy() {
	DeleteGO(m_font);
}

void CVEditor::Update() {
	if (!m_isEditorActive) return;

	if (ZKeyBoard().GetStateTracker().pressed.Back){
		if(m_str.length() > 0)
			m_str.erase(m_str.length() - 1);
	}

	if (m_str.length() > m_numMaxChar) return;

	if (ZKeyBoard().GetStateTracker().pressed.A)
		m_str += L"A";
	if (ZKeyBoard().GetStateTracker().pressed.B)
		m_str += L"B";
	if (ZKeyBoard().GetStateTracker().pressed.C)
		m_str += L"C";
	if (ZKeyBoard().GetStateTracker().pressed.D)
		m_str += L"D";
	if (ZKeyBoard().GetStateTracker().pressed.E)
		m_str += L"E";
	if (ZKeyBoard().GetStateTracker().pressed.F)
		m_str += L"F";
	if (ZKeyBoard().GetStateTracker().pressed.G)
		m_str += L"G";
	if (ZKeyBoard().GetStateTracker().pressed.H)
		m_str += L"H";
	if (ZKeyBoard().GetStateTracker().pressed.I)
		m_str += L"I";
	if (ZKeyBoard().GetStateTracker().pressed.J)
		m_str += L"J";
	if (ZKeyBoard().GetStateTracker().pressed.K)
		m_str += L"K";
	if (ZKeyBoard().GetStateTracker().pressed.L)
		m_str += L"L";
	if (ZKeyBoard().GetStateTracker().pressed.M)
		m_str += L"M";
	if (ZKeyBoard().GetStateTracker().pressed.N)
		m_str += L"N";
	if (ZKeyBoard().GetStateTracker().pressed.O)
		m_str += L"O";
	if (ZKeyBoard().GetStateTracker().pressed.P)
		m_str += L"P";
	if (ZKeyBoard().GetStateTracker().pressed.Q)
		m_str += L"Q";
	if (ZKeyBoard().GetStateTracker().pressed.R)
		m_str += L"R";
	if (ZKeyBoard().GetStateTracker().pressed.S)
		m_str += L"S";
	if (ZKeyBoard().GetStateTracker().pressed.T)
		m_str += L"T";
	if (ZKeyBoard().GetStateTracker().pressed.U)
		m_str += L"U";
	if (ZKeyBoard().GetStateTracker().pressed.V)
		m_str += L"V";
	if (ZKeyBoard().GetStateTracker().pressed.W)
		m_str += L"W";
	if (ZKeyBoard().GetStateTracker().pressed.X)
		m_str += L"X";
	if (ZKeyBoard().GetStateTracker().pressed.Y)
		m_str += L"Y";
	if (ZKeyBoard().GetStateTracker().pressed.Z)
		m_str += L"Z";

	m_font->SetText(m_str);

}