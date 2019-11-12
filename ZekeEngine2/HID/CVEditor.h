#pragma once
class CVEditor : public GameObject
{
public:
	bool Start();
	void OnDestroy();
	void Update();
	/// <summary>
	/// エディターの設定
	/// </summary>
	/// <param name="info">CVEditor::EditorInfoにフォントの情報を入れる</param>
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
	FontRender* m_font = nullptr;
	FontRender::FontInfo m_fontInfo;
	std::wstring m_str = L"";
	bool m_isEditorActive = true;
};

