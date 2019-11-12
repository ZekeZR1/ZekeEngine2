#pragma once
class FontRender : public GameObject
{
public:
	struct FontInfo {
		CVector2 pos = CVector2::Zero();
		float rot = 0.0f;
		float scale = 1.f;
		CVector2 pivot = { 0.5f,0.5f };
		CVector4 col = CVector4::White;
	};
	FontRender();
	~FontRender();
	bool Start() override;
	void Update() override;
	void PostRender() override;

	void Init(const wchar_t* text = nullptr,
		CVector2 pos = { 0.f,0.f },
		float rot = 0.f,
		CVector4 col = CVector4::White,
		float scale = 1.f,
		CVector2 pivot = { 0.5f,0.5f }
	);

	void SetFontInfo(FontRender::FontInfo info) {
		m_fontInfo.pos = info.pos;	
		m_fontInfo.col = info.col;
		m_fontInfo.pivot = info.pivot;
		m_fontInfo.rot = info.rot;
		m_fontInfo.scale = info.scale;
	}

	//テキストを更新
	void SetText(const wchar_t* text = nullptr);
	void SetText(std::wstring text) {
		wcscpy(m_text, text.c_str());
	}

	void SetPosition(CVector2 pos) {
		m_fontInfo.pos = pos;
	}
	CVector2 GetPosition() {
		return m_fontInfo.pos;
	}
	void SetRotation(float rot) {
		m_fontInfo.rot = rot;
	}
	void SetColor(CVector4 col) {
		m_fontInfo.col = col;
	}

	void SetTextType(CFont::TextType type) {
		m_font.SetTextType(type);
		m_textType = type;
	}


	void DrawShadow(CVector2 offset = { 5,-5 }, float size = 1.f, float rot = 0.f, CVector4 color = {0.f,0.f,0.f,1.f});
private:
	CFont m_font;
	FontInfo m_fontInfo;

	wchar_t  m_text[256];
	//CVector2 m_pos = CVector2::Zero();
	//CVector4 m_color = CVector4::White;
	//float m_scale = 1.f;
	//float m_rot = 0.f;
	//CVector2 m_pivot = { 0.5f,0.5f };

	CFont::TextType m_textType = CFont::en_Alphabet;

	bool m_isDrawShadow = false;
	CFont* m_Shadow = nullptr;
	CVector2* m_Soffset = nullptr;
	float* m_Ssize = nullptr;
	float* m_Srot = nullptr;
	CVector4* m_Scolor = nullptr;
};

