#pragma once
#include "Noncopyable.h"

class CShader : Noncopyable
{
public:
	enum class EnType {
		VS,			//!<頂点シェーダー。
		PS,			//!<ピクセルシェーダー。
		CS,			//!<コンピュートシェーダー。
	};
	CShader();
	~CShader();
	/*!
	* @brief	開放。
	* @details
	*	この関数はデストラクタから自動的に呼ばれます。明示的なタイミングで開放を行いたい場合に使用してください。
	*/
	void Release();
	/*!
	* @brief	ロード。
	*@param[in]	filePath		ファイルパス。
	*@param[in]	entryFuncName	エントリーポイントとなる関数の名前。
	*@param[in]	shaderType		シェーダータイプ。
	*@return	falseが返ってきたらロード失敗。
	*/
	bool Load(const char* filePath, const char* entryFuncName, EnType shaderType);
	/*!
	* @brief
	*/
	inline void* GetBody()
	{
		return m_shader;
	}
	/*!
	* @brief	インプットレイアウトを取得。
	*/
	inline ID3D11InputLayout* GetInputLayout()
	{
		return m_inputLayout;
	}
	inline void* GetByteCode()
	{
		return m_blobOut->GetBufferPointer();
	}
	inline size_t GetByteCodeSize()
	{
		return m_blobOut->GetBufferSize();
	}

private:
	void*				m_shader = nullptr;					//!<シェーダー。
	ID3D11InputLayout*	m_inputLayout = nullptr;			//!<入力レイアウト。
	ID3DBlob*			m_blobOut = nullptr;
};

