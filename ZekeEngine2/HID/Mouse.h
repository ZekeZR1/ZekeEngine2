#pragma once
enum enMouseEve {
	enLeftClick,
	enMiddleClick,
	enRightClick,
	enNotchUp,
	enNotchDown,
	enNumMouseEve
};
namespace Mouse {
	//TODO : ����m�F�܂�
	int GetMouseNotch();
	void UpdateMouseInput();
	bool IsTrigger(enMouseEve);
	bool IsPress(enMouseEve);
	CVector3 GetCursorPos();
}
