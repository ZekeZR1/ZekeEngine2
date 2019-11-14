#include "stdafx.h"
#include "TitleMenu.h"

bool TitleMenu::Start() {
	static CVector2 fixFont = { -130,-40 };
	auto mpos = m_menuTopPos;
	m_selectingButtonSp = NewGO<SpriteRender>(1);
	m_selectingButtonSp->Init(L"Assets/sprite/menuButtonSelected.dds", 300, 60);
	for (int i = 0; i < enNumMenu; i++) {
		//sprite
		m_menus.push_back(NewGO<SpriteRender>(0));
		m_menus[i]->Init(L"Assets/sprite/menuButton.dds", 300, 60, true);
		m_menus[i]->SetPosition(mpos);
		mpos.y += m_menuDist;
		//font
		m_fonts.push_back(NewGO<FontRender>(2));
		FontRender::FontInfo info;
		info.pos = { mpos.x + fixFont.x,mpos.y  + fixFont.y};
		m_fonts[i]->SetFontInfo(info);
	}

	m_fonts[enOnlineMatch]->SetText(L"Online");
	m_fonts[enTraining]->SetText(L"Training");
	m_fonts[enOption]->SetText(L"Option");

	return true;
}

void TitleMenu::OnDestroy() {
	for (auto i : m_menus)
		DeleteGO(i);
	for (auto i : m_fonts)
		DeleteGO(i);
	DeleteGO(m_selectingButtonSp);
}

void TitleMenu::Update() {
	if (Pad(0).IsTrigger(enButtonDown)) {
		if(m_selecting > 0)
			m_selecting--;
	}

	if (Pad(0).IsTrigger(enButtonUp)) {
		if(m_selecting < enNumMenu -1 )
			m_selecting++;
	}

	m_selectingButtonSp->SetPosition(m_menus[m_selecting]->GetPosition());
}
