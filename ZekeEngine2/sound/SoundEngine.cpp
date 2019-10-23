#include "stdafx.h"
#include "SoundEngine.h"


SoundEngine::SoundEngine() {
	// This is only needed in Windows desktop apps
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<DirectX::AudioEngine>(eflags);
}

SoundEngine::~SoundEngine() {

}

void SoundEngine::Update() {
	if (!m_audEngine->Update())
	{
		// No audio device is active
		if (m_audEngine->IsCriticalError())
		{
		}
	}
}

