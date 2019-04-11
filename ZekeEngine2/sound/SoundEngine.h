#pragma once

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();
	void Update();
//private:
	std::unique_ptr<DirectX::AudioEngine> audEngine;
};

static SoundEngine& ISoundEngine() {
	static SoundEngine soundEngine;
	return soundEngine;
}

