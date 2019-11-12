#pragma once
class CVEditor : public GameObject
{
public:
	bool Start();
	void OnDestroy();
	void Update();
	void SetEditorActive(bool isActive) {
		m_isEditorActive = isActive;
	}
	const std::string const GetString() {
		return m_str;
	}
private:
	std::string m_str;
	bool m_isEditorActive = true;
};

