#include "stdafx.h"
#include "Primitive.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
	Release();
}

bool Primitive::Create(
	D3D_PRIMITIVE_TOPOLOGY topology,
	int numVertex,
	int vertexStride,
	void* pSrcVertexBuffer,
	int numIndex,
	CIndexBuffer::EnIndexType indexType,
	void* pSrcIndexBuffer
) {
	Release();
	m_topology = topology;
	bool result = m_vertexBuffer.Create(numVertex, vertexStride, pSrcVertexBuffer);
	if (!result) {
		assert(false && "create vertex buffer failed\n");
		return false;
	}
	if (pSrcIndexBuffer) {
		result = m_indexBuffer.Create(numIndex, indexType, pSrcIndexBuffer);
		if (!result) {
			assert(false && "create index buffer failed\n");
			return false;
		}
	}
	return true;
}

void Primitive::Release() {
	m_vertexBuffer.Release();
	m_indexBuffer.Release();
}

void Primitive::Draw() {
	auto ge = g_graphicsEngine;
	UINT offset = 0;
	UINT stride = m_vertexBuffer.GetStride();
	ge->GetD3DDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_vertexBuffer.GetBody(),
		&stride,
		&offset
	);
	ge->GetD3DDeviceContext()->IASetIndexBuffer(
		m_indexBuffer.GetBody(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	ge->GetD3DDeviceContext()->IASetPrimitiveTopology(m_topology);
	ge->GetD3DDeviceContext()->DrawIndexed(m_indexBuffer.GetNumIndex(), 0, 0);
}

void Primitive::Draw(int numVertex) {
	auto ge = g_graphicsEngine;
	UINT offset = 0;
	UINT stride = m_vertexBuffer.GetStride();
	ge->GetD3DDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_vertexBuffer.GetBody(),
		&stride,
		&offset
	);
	ge->GetD3DDeviceContext()->IASetPrimitiveTopology(m_topology);
	ge->GetD3DDeviceContext()->Draw(numVertex, 0);

}