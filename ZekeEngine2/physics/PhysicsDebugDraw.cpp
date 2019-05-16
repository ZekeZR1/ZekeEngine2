#include "stdafx.h"
#include "PhysicsDebugDraw.h"

void PhysicsDebugDraw::Init() {
	m_primitive.Create(
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		static_cast<int>(m_vertexBuffer.size()),
		sizeof(CVector4),
		&m_vertexBuffer[0],
		0,
		CIndexBuffer::enIndexType_32,
		nullptr
	);
	m_vs.Load("Assets/shader/linePrimitive.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/linePrimitive.fx", "PSMain", Shader::EnType::PS);
	InitConstantBuffer();
}

void PhysicsDebugDraw::Release() {
	if (m_cb != nullptr) {
		m_cb->Release();
	}
}
void PhysicsDebugDraw::InitConstantBuffer() {
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = (((sizeof(SConstantBuffer) - 1) / 16) + 1) * 16;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	GraphicsEngine().GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_cb);
	assert(m_cb != nullptr);
}

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color){
	int baseIndex = m_numLine * 2;
	m_vertexBuffer[baseIndex].Set(CVector3(from.x(), from.y(), from.z()));
	m_vertexBuffer[baseIndex + 1].Set(CVector3(to.x(), to.y(), to.z()));
	m_numLine++;
	/*char str[256];
	sprintf_s(str, "from.x ...%f, y... %f, z... %f\n",from.getX(),from.getY(),from.getZ());
	OutputDebugStringA(str);*/
}

void PhysicsDebugDraw::EndDraw() {
	SConstantBuffer cb;
	cb.mView = MainCamera().GetViewMatrix();
	cb.mProj = MainCamera().GetProjectionMatrix();
	//g_graphicsEngine->GetD3DDeviceContext()->UpdateSubresource(m_cb, &cb);
	//m_renderContext->UpdateSubresource(m_cb, &cb);
	GraphicsEngine().GetD3DDeviceContext()->VSSetShader((ID3D11VertexShader*	)m_vs.GetBody(), nullptr, 0);
	GraphicsEngine().GetD3DDeviceContext()->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), nullptr, 0);
	GraphicsEngine().GetD3DDeviceContext()->UpdateSubresource(m_primitive.GetVertexBuffer().GetBody(), 0, NULL, &m_vertexBuffer[0], 0, 0);
	GraphicsEngine().GetD3DDeviceContext()->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
	GraphicsEngine().GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, &m_cb);
	GraphicsEngine().GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, &m_cb);
	m_primitive.Draw(m_numLine * 2);
	//ge->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	/*m_renderContext->VSSetShader(m_vs);
	m_renderContext->PSSetShader(m_ps);
	m_renderContext->VSSetConstantBuffer(0, m_cb);*/
	//m_renderContext->UpdateSubresource(m_primitive.GetVertexBuffer(), &m_vertexBuffer[0]);
	//m_primitive.Draw(*m_renderContext, m_numLine * 2);
}