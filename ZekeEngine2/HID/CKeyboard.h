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

	//キー入力を取得する際はkeyboard.getstateの戻り値を使って調べる
	const std::unique_ptr<DirectX::Keyboard>& const GetKeyboard() {
		return m_keyboard;
	}

	//キーのトリガーを取得する際はtracker.press.???の戻り値
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