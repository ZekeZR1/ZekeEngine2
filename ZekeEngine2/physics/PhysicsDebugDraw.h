#pragma once
#include <array>
#include "../graphics/Primitive.h"

class PhysicsDebugDraw : public btIDebugDraw {
	struct SConstantBuffer {
		CMatrix mView;
		CMatrix mProj;
	};
	void InitConstantBuffer();
	int m_debugMode = btIDebugDraw::DBG_NoDebug;
	int m_numLine = 0;
	ID3D11Buffer* m_cb = nullptr;
	Primitive m_primitive;
	Shader m_vs;
	Shader m_ps;
	static const int VERTEX_MAX = 1000000;
	std::array<CVector4, VERTEX_MAX> m_vertexBuffer;
public:
	void BeginDraw() {
		m_numLine = 0;
	}
	void Init();
	void Release();
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void EndDraw();
	void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override
	{
	}
	void reportErrorWarning(const char*) override
	{
	}
	void draw3dText(const btVector3&, const char*) override
	{
	}
	void setDebugMode(int debugMode) override
	{
		m_debugMode = debugMode;
	}
	int	getDebugMode() const override
	{
		return m_debugMode;
	}
};

