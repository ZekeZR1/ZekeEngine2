#include "stdafx.h"
#include "FontRender.h"



FontRender::FontRender()
{
}


FontRender::~FontRender()
{
	if (m_isDrawShadow)
	{
		delete m_Shadow;
		delete m_Scolor;
		delete m_Soffset;
		delete m_Srot;
		delete m_Ssize;
	}
}

bool FontRender::Start() {
	return true;
}

void FontRender::Update() {

}

void FontRender::PostRender() {
	if (m_isDrawShadow)
	{
		m_Shadow->Begin();
		m_Shadow->Draw(m_text, m_fontInfo.pos + *m_Soffset, *m_Scolor, m_fontInfo.rot + *m_Srot, m_fontInfo.scale * *m_Ssize, m_fontInfo.pivot);
		m_Shadow->End();
	}

	m_font.Begin();
	m_font.Draw(m_text, m_fontInfo.pos, m_fontInfo.col, m_fontInfo.rot, m_fontInfo.scale, m_fontInfo.pivot);
	m_font.End();
	
}

void FontRender::Init(
	const wchar_t* text,
	CVector2 pos ,
	float rot ,
	CVector4 col,
	float scale,
	CVector2 pivot
	) {
	//m_text = text;
	wcscpy(m_text,text);
	m_fontInfo.pos = pos;
	m_fontInfo.rot = rot;
	m_fontInfo.col = col;
	m_fontInfo.scale = scale;
	m_fontInfo.pivot = pivot;
}

void FontRender::SetText(const wchar_t* text) {
	wcscpy(m_text, text);
}


void FontRender::DrawShadow(CVector2 offset, float size, float rote, CVector4 color)
{
	
	m_Shadow = new CFont();
	m_Shadow->SetTextType(m_textType);

	m_Soffset = new CVector2(offset);
	m_Ssize = new float(size);
	m_Srot = new float(rote);
	m_Scolor = new CVector4(color);

	m_isDrawShadow = true;
}
