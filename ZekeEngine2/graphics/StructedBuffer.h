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
	* @brief	ID3D11Buffer*���擾�B
	*/
	ID3D11Buffer*& GetBody()
	{
		return m_structuredBuffer;
	}
	/*!
	*@brief	�J���B
	*@details
	* ���̊֐��̓f�X�g���N�^������R�[������Ă��܂��B
	* �����I�ȃ^�C�~���O�ŊJ���������ꍇ�ɌĂяo���Ă��������B
	*/
	void Release();
	/*!
	* @brief	SRV���擾�B
	*/
	ShaderResouceView &GetSRV(){
		return m_srv;
	}
	/*!
	* @brief	UAV���擾�B
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

