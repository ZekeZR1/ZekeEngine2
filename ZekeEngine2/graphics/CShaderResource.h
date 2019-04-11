#pragma once
#include "CShader.h"
#include "TScopedResource.h"
#include "Noncopyable.h"

class CShader;

class CShaderResource : Noncopyable
{
public:
	CShaderResource();
	~CShaderResource();
	bool Load(
		void*& shader,
		ID3D11InputLayout*& inputLayout,
		ID3DBlob*& blob,
		const char* filePath,
		const char* entryFuncName,
		CShader::EnType shaderType
	);
	/*!
	*@brief	�J���B
	*@details
	* �����I�ȃ^�C�~���O�ŊJ���������ꍇ�Ɏg�p���Ă��������B
	*/
	void Release();
private:
	struct SShaderProgram {
		std::unique_ptr<char[]> program;
		int programSize;
	};
	struct SShaderResource {
		void* shader;					//!<�V�F�[�_�[�B
		ID3D11InputLayout* inputLayout;	//!<���̓��C�A�E�g�B
		CShader::EnType type;			//!<�V�F�[�_�[�^�C�v�B
		TScopedResource<ID3DBlob> blobOut;
	};
	typedef std::unique_ptr<SShaderResource>	SShaderResourcePtr;
	typedef std::unique_ptr<SShaderProgram>	SShaderProgramPtr;
	std::map<int, SShaderProgramPtr>		m_shaderProgramMap;		//!<�ǂݍ��ݍς݂̃V�F�[�_�[�v���O�����̃}�b�v�B
	std::map<int, SShaderResourcePtr>	m_shaderResourceMap;	//!<�V�F�[�_�[���\�[�X�̃}�b�v
};

