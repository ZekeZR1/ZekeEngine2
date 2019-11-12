#pragma once
class CKeyboard : Noncopyable
{
public:
	CKeyboard();
	void Update();

	static CKeyboard& GetInstance() {
		static CKeyboard key;
		return key;
	}

	//�L�[���͂��擾����ۂ�keyboard.getstate�̖߂�l���g���Ē��ׂ�
	const std::unique_ptr<DirectX::Keyboard>& const GetKeyboard() {
		return m_keyboard;
	}

	//�L�[�̃g���K�[���擾����ۂ�tracker.press.???�̖߂�l
	const DirectX::Keyboard::KeyboardStateTracker& const GetStateTracker(){
		return m_tracker;
	}

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	DirectX::Keyboard::KeyboardStateTracker m_tracker;
};

static inline CKeyboard& const ZKeyBoard() {
	return CKeyboard::GetInstance();
}