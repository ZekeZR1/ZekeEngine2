#pragma once
class FontRender : public GameObject
{
public:
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

	void SetPosition(CVector2 pos) {
		m_pos = pos;
	}
	CVector2 GetPosition() {
		return m_pos;
	}
	void SetRotation(float rot) {
		m_rot = rot;
	}
	void SetColor(CVector4 col) {
		m_color = col;
	}

	void SetTextType(CFont::TextType type) {
		m_font.SetTextType(type);
		m_textType = type;
	}

	//�ĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕ�
	//�ĕĕā@�K���A��΂ɁA���������Ă��AInit�֐���SetTextType�֐��̂��Ƃɂ����ĂˁB���Z�l�Ƃ̂��񑩏�������H
	//�ĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕĕ�
	//�e��`�悵�邷�B
	//arg: ��{�I�ɂ����Őݒ肷����̂͑��ΓI�Ȑ����ށB
	//	offset: �t�H���g�����_�[�����̃|�W�V�����Ƀv���X����܂���B
	//	size: �`�掞�Ɍ��X�̃t�H���g�����_�[�̃T�C�Y�Ɋ|����
	//	rot: �`�掞�Ɍ��̃t�H���g�̉�]�Ƀv���X�����
	//	color: �e�̐F�B
	void DrawShadow(CVector2 offset = { 5,-5 }, float size = 1.f, float rot = 0.f, CVector4 color = {0.f,0.f,0.f,1.f});
private:
	CFont m_font;
	wchar_t  m_text[256];
	//wchar_t * m_returnString;
	CVector2 m_pos = CVector2::Zero();
	//CQuaternion m_rot = CQuaternion::Identity();
	CVector4 m_color = CVector4::White;
	float m_scale = 1.f;
	float m_rot = 0.f;
	CVector2 m_pivot = { 0.5f,0.5f };

	CFont::TextType m_textType = CFont::en_Alphabet;

	bool m_isDrawShadow = false;
	CFont* m_Shadow = nullptr;
	CVector2* m_Soffset = nullptr;
	float* m_Ssize = nullptr;
	float* m_Srot = nullptr;
	CVector4* m_Scolor = nullptr;
};

