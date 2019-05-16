// ñºÇÕGameÇ∆ê\Ç∑ÅB

#include "stdafx.h"
#include "Game.h"


bool Game::Start() {
	m_model = NewGO<SkinModelRender>(0);
	m_model->Init(L"Assets/modelData/egg.cmo");
	m_model->SetPosition(CVector3::Zero());
	return true;
}

void Game::OnDestroy() {
	
}

void Game::Update() {
	
}
