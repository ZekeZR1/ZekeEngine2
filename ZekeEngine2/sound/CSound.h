#pragma once

class Sound : public GameObject {
public:
	Sound();
	~Sound();
	bool Start() override;
	void Update() override;;
	void Init(const wchar_t*, bool = false);
	void Play();
	bool isPlaying();
	void Stop();
	void SetVolume(float vol);
	void SetPitch(float p) {
		pitch = p;
	}
	void SetPan(float pa) {
		pan = pa;
	}
private:
	bool m_isInited = false;
	bool m_roofFlag = false;
	float volume = 1.f;
	float pitch = 0.f;
	float pan = 0.f;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> effect;
};

