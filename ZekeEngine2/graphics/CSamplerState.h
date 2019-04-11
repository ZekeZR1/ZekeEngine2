#pragma once
class CSamplerState : Noncopyable
{
public:
	CSamplerState();
	~CSamplerState();
	bool Create(const D3D11_SAMPLER_DESC& desc);
	/*!
	* @brief	明示的な開放処理。
	*/
	void Release()
	{
		if (m_samplerState != nullptr) {
			m_samplerState->Release();
			m_samplerState = nullptr;
		}
	}
	/*!
	* @brief	サンプラステートを取得。
	*/
	ID3D11SamplerState*& GetBody()
	{
		return m_samplerState;
	}
private:
	ID3D11SamplerState * m_samplerState = nullptr;
};

