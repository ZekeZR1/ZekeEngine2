#pragma once
enum enMouseEve {
	enLeftClick,
	enMiddleClick,
	enRightClick,
	enNotchUp,
	enNotchDown,
	enNumMouseEve
	//TODO : �}�E�X�C�x���g��enNotchUp down�ǉ�����trigger����悤�ɂ���
};
namespace Mouse {
	int GetMouseNotch();
	void UpdateMouseInput();
	bool isTrigger(enMouseEve);
	bool isPress(enMouseEve);
	CVector3 GetCursorPos();
}
