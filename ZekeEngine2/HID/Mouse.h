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
	//TODO : “®ìŠm”F‚Ü‚¾
	int GetMouseNotch();
	void UpdateMouseInput();
	bool IsTrigger(enMouseEve);
	bool IsPress(enMouseEve);
	CVector3 GetCursorPos();
}
