#include "stdafx.h"
#include "CSound.h"
#include "SoundEngine.h"

Sound::Sound() {
	
}

Sound::~Sound() {
}

bool Sound::Start() {
	return true;
}

void Sound::Update() {
	if (m_roofFlag)
		effect->Play(true);

	if (!isPlaying()) {
		OutputDebugStringA("Delete Sound effect");
		DeleteGO(this);
	}
}

void Sound::Init(const wchar_t* filepath, bool roopflag) {
	m_roofFlag = roopflag;
	if (roopflag) {
		soundEffect = std::make_unique<DirectX::SoundEffect>(ISoundEngine().GetAudioEngine().get(), filepath);
		char hoge[256];
		sprintf(hoge, "filePath = %s Engine Addr = %lx\n", ISoundEngine().GetAudioEngine().get(), filepath);
		OutputDebugStringA(hoge);
		effect = soundEffect->CreateInstance();
	}
	else {
		soundEffect = std::make_unique<DirectX::SoundEffect>(ISoundEngine().GetAudioEngine().get(), filepath);
		char hoge[256];
		sprintf(hoge, "filePath = %s Engine Addr = %lx\n", ISoundEngine().GetAudioEngine().get(), filepath);
		OutputDebugStringA(hoge);
	}
	m_isInited = true;
}

void Sound::Play() {
	if (m_roofFlag) {
		effect->SetVolume(volume);
		effect->SetPan(pan);
		effect->SetPitch(pitch);
		effect->Play(true);
	}
	else {
		soundEffect->Play(volume, pitch, pan);
	}
}

void Sound::Stop() {
	DeleteGO(this);
}

bool Sound::isPlaying() {
	return soundEffect->IsInUse();
}

void Sound::SetVolume(float vol) {
	volume = vol;
	if (m_roofFlag)
		effect->SetVolume(vol);
}