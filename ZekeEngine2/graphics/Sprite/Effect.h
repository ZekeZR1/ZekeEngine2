#pragma once
class Effect
{
public:
	Effect();
	~Effect();
	void Load(const char* shaderFilePath);
	void BeginRender();
private:
	ID3D11VertexShader * m_vs = NULL;	
	ID3D11PixelShader* m_ps = NULL;	
	ID3DBlob* m_vsBlobOut = NULL;
	ID3DBlob* m_psBlobOut = NULL;
	ID3D11InputLayout* m_inputLayout = NULL;

};

