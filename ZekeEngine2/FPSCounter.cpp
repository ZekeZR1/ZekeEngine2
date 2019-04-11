#include "stdafx.h"
#include "FPSCounter.h"

// �R���X�g���N�^
CFPSCounter::CFPSCounter(unsigned int smp)
{

	//
	//pSpriteBatch = new DirectX::SpriteBatch(g_graphicsEngine->GetD3DDeviceContext());
	//pSpriteFont = new DirectX::SpriteFont(g_graphicsEngine->GetD3DDevice(), L"Assets/font/myfile.spritefont");
	// �T���v�����̐ݒ�
	SetSampleNum(smp);

	// �v�����鎞�v�̑I��
	if (QueryPerformanceCounter(&m_Counter) != 0)
	{
		// QueryPerformanceCounter�֐����g���t���O
		m_iCounterFlag = FPSCOUNTER_QUERYPER_COUNTER;
		m_OldLongCount = m_Counter.QuadPart;        // �������̎����i�N���b�N���j���擾
		LARGE_INTEGER Freq;
		QueryPerformanceFrequency(&Freq);            // 1�b������N���b�N�����擾
		m_dFreq = (double)Freq.QuadPart;
	}
	else
	{
		// timeGetTime�֐����g���t���O
		m_iCounterFlag = FPSCOUNTER_TIMEGETTIME;

		// ���x���グ��
		timeBeginPeriod(1);

		// �������̎����i�~���b�j���擾
		m_dwTGTOldCount = timeGetTime();
	}

	// �v��
	GetFPS();

	m_font = NewGO<FontRender>(31, "fontrender");
}


// �f�X�g���N�^
CFPSCounter::~CFPSCounter(void)
{
	//delete pSpriteBatch;
	//delete pSpriteFont;
	DeleteGO(m_font);
	if (m_iCounterFlag == FPSCOUNTER_TIMEGETTIME)
		timeEndPeriod(1);    // �^�C�}�[�̐��x��߂�
}


// FPS�l���擾
double CFPSCounter::GetFPS()
{
	double Def = GetCurDefTime();
	if (Def == 0) {
		// �v�Z�ł��Ȃ��̂ł�Ԃ�
		return 0;
	}

	return UpdateFPS(Def);
}


// ���݂̍����������~���b�P�ʂŎ擾
double CFPSCounter::GetCurDefTime()
{
	// �������Ԃ��v��
	if (m_iCounterFlag == FPSCOUNTER_QUERYPER_COUNTER)
	{
		// QueryPerformanceCounter�֐��ɂ��v��
		QueryPerformanceCounter(&m_Counter);                     // ���݂̎������擾���A
		LONGLONG LongDef = m_Counter.QuadPart - m_OldLongCount;    // �����J�E���g�����Z�o����B
		double dDef = (double)LongDef;                             // �{���x���������_�ɕϊ�
		m_OldLongCount = m_Counter.QuadPart;                       // ���݂̎�����ێ�
		return dDef * 1000 / m_dFreq;                                // �������Ԃ��~���b�P�ʂŕԂ�
	}

	// timeGetTime�֐��ɂ��v��
	DWORD CurTime = timeGetTime();
	DWORD CurDef = CurTime - m_dwTGTOldCount;         // �����J�E���g�����Z�o����
	m_dwTGTOldCount = CurTime;                        // ���݂̎�����ێ�
	return CurDef;
}


// FPS���X�V
double CFPSCounter::UpdateFPS(double Def)
{
	// �ł��Â��f�[�^������
	m_dwDefTimeLst.pop_front();

	// �V�����f�[�^��ǉ�
	m_dwDefTimeLst.push_back(Def);

	// FPS�Z�o
	double FPS;
	double AveDef = (m_dwSumTimes + Def) / m_uiNum;
	if (AveDef != 0)
		FPS = 1000.0 / AveDef;

	// ���ʉ��Z�����̍X�V
	m_dwSumTimes += Def - *m_dwDefTimeLst.begin();

	return FPS;
}


// �T���v������ύX
void CFPSCounter::SetSampleNum(unsigned int smp)
{
	m_uiNum = smp;    // ���ς��v�Z�����
	m_dwDefTimeLst.resize(m_uiNum, 0.0);    // ���X�g������
	m_dwSumTimes = 0;
}

void CFPSCounter::Draw() {
	double FrameRate = GetFPS();
	wchar_t str[256];
	swprintf_s(str, L"%.2f\n", FrameRate);
	m_font->Init(str, { -630.f , 350.f }, 0.f, CVector4::Green, 1.f, { 0.0f,0.0f });
	/*
	pSpriteBatch->Begin();
	fps = GetFPS();
	_itow_s(fps, wfps, 10);
	pSpriteFont->DrawString(pSpriteBatch, (L"%d", wfps), DirectX::XMFLOAT2(1220.0f, 0.0f), CVector4::Green, 0.0f, DirectX::XMFLOAT2(0.0, 0.0), 1.0f);
	pSpriteBatch->End();
	*/
}