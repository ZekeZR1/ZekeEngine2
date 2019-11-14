#pragma once
class CVEditor : public GameObject
{
public:
	bool Start();
	void OnDestroy();
	void Update();
	void CreateText(int updatePriority);
	/// <summary>
	/// �G�f�B�^�[�̐ݒ�
	/// </summary>
	/// <param name="info">CVEditor::EditorInfo�Ƀt�H���g�̏�������</param>
	void SetEditorInfo(FontRender::FontInfo info) {
		m_fontInfo = info;
		if(m_font!=nullptr)
			m_font->SetFontInfo(info);
	}
	void SetEditorActive(bool isActive) {
		m_isEditorActive = isActive;
	}
	const std::wstring const GetString() {
		return m_str;
	}

private:
	int m_numMaxChar = 5;
	FontRender* m_font = nullptr;
	FontRender::FontInfo m_fontInfo;
	std::wstring m_str = L"";
	bool m_isEditorActive = true;
};

