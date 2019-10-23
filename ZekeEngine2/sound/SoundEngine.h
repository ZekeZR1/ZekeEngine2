#pragma once

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();
	void Update();
	std::unique_ptr<DirectX::AudioEngine>& GetAudioEngine() {
		return m_audEngine;
	}
private:
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
};

static SoundEngine& ISoundEngine() {
	static SoundEngine soundEngine;
	return soundEngine;
}

