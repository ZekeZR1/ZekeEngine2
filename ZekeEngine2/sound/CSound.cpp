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
		soundEffect = std::make_unique<DirectX::SoundEffect>(ISoundEngine().audEngine.get(), filepath);
		char hoge[256];
		sprintf(hoge, "filePath = %s Engine Addr = %lx\n", ISoundEngine().audEngine.get(), filepath);
		OutputDebugString(hoge);
		effect = soundEffect->CreateInstance();
	}
	else {
		soundEffect = std::make_unique<DirectX::SoundEffect>(ISoundEngine().audEngine.get(), filepath);
		char hoge[256];
		sprintf(hoge, "filePath = %s Engine Addr = %lx\n", ISoundEngine().audEngine.get(), filepath);
		OutputDebugString(hoge);
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
//	effect->Stop();
	DeleteGO(this);
}

bool Sound::isPlaying() {
	if (soundEffect->IsInUse()) {
		return true;
		/*
		char message[256];
		sprintf_s(message, "Playing sound\n");
		OutputDebugStringA(message);
		*/
	}
	else {
		return false;
		/*
		char message[256];
		sprintf_s(message, "Not playing sound\n");
		OutputDebugStringA(message);
		*/
	}
}

void Sound::SetVolume(float vol) {
	volume = vol;
	if (m_roofFlag)
		effect->SetVolume(vol);
}