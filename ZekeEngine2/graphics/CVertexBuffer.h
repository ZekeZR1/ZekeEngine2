#pragma once
class CVertexBuffer : Noncopyable
{
public:
	CVertexBuffer();
	~CVertexBuffer();
	bool Create(int numVertex, int stride, const void* pSrcVertexBuffer);
	/*!
	*@brief	開放。
	*@details
	* この関数はデストラクタからもコールされています。
	* 明示的なタイミングで開放したい場合に呼び出してください。
	*/
	void Release();
	/*!
	*@brief	頂点ストライドを取得。
	*/
	int GetStride() const
	{
		return m_stride;
	}
	/*!
	*@brief ID3D11Buffer*を取得。
	*/
	ID3D11Buffer*& GetBody()
	{
		return m_vertexBuffer;
	}
private:
	ID3D11Buffer * m_vertexBuffer = nullptr;	//!<頂点バッファ。
	int				m_stride = 0;				//!<頂点ストライド。
};
