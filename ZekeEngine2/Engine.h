#pragma once
class Engine : Noncopyable
{
private:
	Engine();
	~Engine();
public:
	void Init(HINSTANCE, HINSTANCE, LPWSTR, int);
	void Final();
	void GameRoop();
	static Engine& GetInstance() {
		/*static Engine* instance = nullptr;
		if (instance == nullptr) {
			instance = new Engine;
		}
		return *instance;*/
		static Engine instance;
		return instance;
	}
private:
	//static Engine* instance;
	void Update();
	void Release();
public:
	static inline Engine& IEngine()
	{
		return Engine::GetInstance();
	}
};


