#pragma once
class Primitive
{
public:
	Primitive();
	~Primitive();
	bool Create(
		D3D_PRIMITIVE_TOPOLOGY topology,
		int numVertex,
		int vertexStride,
		void* pSrcVertexBuffer,
		int numIndex,
		CIndexBuffer::EnIndexType indexType,
		void* pSrcIndexBuffer
	);
	void Release();
	void Draw();
	void Draw(int);
	CVertexBuffer& GetVertexBuffer()
	{
		return m_vertexBuffer;
	}
private:
	CVertexBuffer			m_vertexBuffer;	
	CIndexBuffer			m_indexBuffer;		
	D3D_PRIMITIVE_TOPOLOGY	m_topology;	
};

