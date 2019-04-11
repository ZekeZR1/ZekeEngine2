#pragma once
#include "ShaderResouceView.h"
#include "CUnorderedAccessView.h"

class CUnorderedAccessView;
class ID3D11Buffer;
class ShaderResouceView;

class StructedBuffer : Noncopyable
{
public:
	StructedBuffer();
	~StructedBuffer();
	bool Create(const void* pInitData, D3D11_BUFFER_DESC& bufferDesc);
	/*!
	* @brief	ID3D11Buffer*を取得。
	*/
	ID3D11Buffer*& GetBody()
	{
		return m_structuredBuffer;
	}
	/*!
	*@brief	開放。
	*@details
	* この関数はデストラクタからもコールされています。
	* 明示的なタイミングで開放したい場合に呼び出してください。
	*/
	void Release();
	/*!
	* @brief	SRVを取得。
	*/
	ShaderResouceView &GetSRV(){
		return m_srv;
	}
	/*!
	* @brief	UAVを取得。
	*/
	CUnorderedAccessView& GetUAV()
	{
		return m_uav;
	}
private:
	ID3D11Buffer * m_structuredBuffer = nullptr;	//!<StructuredBuffer
	ShaderResouceView m_srv;
	CUnorderedAccessView	m_uav;							//!<UAV
};

