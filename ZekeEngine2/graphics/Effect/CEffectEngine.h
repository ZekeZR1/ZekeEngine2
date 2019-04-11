#pragma once
class CEffectEngine
{
public:
	CEffectEngine();
	~CEffectEngine();
	void Init();
	void Update();
	void Draw();
	Effekseer::Manager& GetEffekseerManager()
	{
		return *m_manager;
	}
	//private:
	Effekseer::Manager*	m_manager = nullptr;
	EffekseerRenderer::Renderer*	m_renderer = nullptr;
	Effekseer::Vector3D			m_position;		//camera pos
};