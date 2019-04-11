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
		static Engine* instance = nullptr;
		if (instance == nullptr) {
			instance = new Engine;
		}
		return *instance;
	}

	NetworkLogic* GetNetworkLogic()
	{
		return m_network;
	}

private:
	void Update();
	void Release();

	NetworkLogic* m_network = nullptr;
public:
	static inline Engine& IEngine()
	{
		return Engine::GetInstance();
	}
	void CreateNetworkSystem();
	void DestroyNetworkSystem();
};


