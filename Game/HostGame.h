#pragma once

class HostGame : public GameObject
{
public:
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
};

