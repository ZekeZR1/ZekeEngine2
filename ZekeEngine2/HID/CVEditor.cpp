#include "stdafx.h"
#include "CVEditor.h"

bool CVEditor::Start() {
	return true;
}

void CVEditor::OnDestroy() {

}

void CVEditor::Update() {
	if (!m_isEditorActive) return;

	if (ZKeyBoard().GetStateTracker().pressed.Back){
		m_str.erase(m_str.length() - 1);
	}
	if (ZKeyBoard().GetStateTracker().pressed.A)
		m_str += "A";
	if (ZKeyBoard().GetStateTracker().pressed.B)
		m_str += "B";
	if (ZKeyBoard().GetStateTracker().pressed.C)
		m_str += "C";
	if (ZKeyBoard().GetStateTracker().pressed.D)
		m_str += "D";
	if (ZKeyBoard().GetStateTracker().pressed.E)
		m_str += "E";
	if (ZKeyBoard().GetStateTracker().pressed.F)
		m_str += "F";
	if (ZKeyBoard().GetStateTracker().pressed.G)
		m_str += "G";
	if (ZKeyBoard().GetStateTracker().pressed.H)
		m_str += "H";
	if (ZKeyBoard().GetStateTracker().pressed.I)
		m_str += "I";
	if (ZKeyBoard().GetStateTracker().pressed.J)
		m_str += "J";
	if (ZKeyBoard().GetStateTracker().pressed.K)
		m_str += "K";
	if (ZKeyBoard().GetStateTracker().pressed.L)
		m_str += "L";
	if (ZKeyBoard().GetStateTracker().pressed.M)
		m_str += "M";
	if (ZKeyBoard().GetStateTracker().pressed.N)
		m_str += "N";
	if (ZKeyBoard().GetStateTracker().pressed.O)
		m_str += "O";
	if (ZKeyBoard().GetStateTracker().pressed.P)
		m_str += "P";
	if (ZKeyBoard().GetStateTracker().pressed.Q)
		m_str += "Q";
	if (ZKeyBoard().GetStateTracker().pressed.R)
		m_str += "R";
	if (ZKeyBoard().GetStateTracker().pressed.S)
		m_str += "S";
	if (ZKeyBoard().GetStateTracker().pressed.T)
		m_str += "T";
	if (ZKeyBoard().GetStateTracker().pressed.U)
		m_str += "U";
	if (ZKeyBoard().GetStateTracker().pressed.V)
		m_str += "V";
	if (ZKeyBoard().GetStateTracker().pressed.W)
		m_str += "W";
	if (ZKeyBoard().GetStateTracker().pressed.X)
		m_str += "X";
	if (ZKeyBoard().GetStateTracker().pressed.Y)
		m_str += "Y";
	if (ZKeyBoard().GetStateTracker().pressed.Z)
		m_str += "Z";
}