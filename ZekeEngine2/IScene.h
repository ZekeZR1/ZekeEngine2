#pragma once
class IScene
{
public:
	IScene(){
	}
	virtual ~IScene() {
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

extern IScene* currentScene;