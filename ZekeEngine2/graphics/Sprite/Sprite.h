#pragma once
#include "Effect.h"
#include "../ShaderResouceView.h"
#include "../CRenderContext.h"
#include "../Camera.h"
#include "../CPrimitive.h"
#include "../CShader.h"
#include "../Shader.h"

class Sprite : Noncopyable
{
public:
	static const CVector2	DEFAULT_PIVOT;					//!<ピボット。

	Sprite();
	~Sprite();
	void Init(ID3D11ShaderResourceView* srv, float w, float h);
	/*
	*@brief	初期化。
	*@param	texFilePath		テクスチャのファイルパス。
	*@param	w				画像の幅。
	*@param	h				画像の高さ。
	*/
	void Init(const wchar_t* texFilePath, float w, float h);
	/*
	*@brief	更新。
	*@param[in]	trans		平行移動。
	*@param[in]	rot			回転。
	*@param[in]	scale		拡大。
	*@param[in]	pivot		基点。
	*						0.5, 0.5で画像の中心が基点。
	*						0.0, 0.0で画像の左下。
	*						1.0, 1.0で画像の右上。
	*						UnityのuGUIに準拠。
	*/
	void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, CVector2 pivot = { 0.5f, 0.5f });
	/*
	*@brief	描画。
	*/
	void Draw();
	void Draww();

	void ChangeCameraProjMatrix(Camera::EnUpdateProjMatrixFunc cameraMode) {
		m_cameraMode = cameraMode;
	}

	void SetMulColor(const CVector4& col) {
		m_mulCol = col;
	}
	struct ConstantBuffer {
		CMatrix WVP;
		CVector4 mulCol;
	};
	Camera::EnUpdateProjMatrixFunc m_cameraMode = Camera::enUpdateProjMatrixFunc_Ortho;
	ID3D11Buffer*				m_vertexBuffer = NULL;				
	ID3D11Buffer*				m_indexBuffer = NULL;				
	ID3D11DepthStencilState* m_depthStencilState = NULL;
	ID3D11DepthStencilState*	spriteRender = NULL;
	ID3D11RasterizerState*	rspriteRender = NULL;

	Effect						m_effect;								
	ID3D11ShaderResourceView*	m_texture = NULL;						
	ID3D11SamplerState*			m_samplerState = NULL;				
	CVector3					m_position = CVector3::Zero();		
	CQuaternion					m_rotation = CQuaternion::Identity();	//回転
	CVector3					m_scale = CVector3::One();
	CMatrix						m_world = CMatrix::Identity();			//ワールド行列。
	CVector2					m_size = CVector2::Zero();				//画像のサイズ。
	ID3D11Buffer*				m__cb = nullptr;							//定数バッファ。
	Shader						m_vs;									//頂点シェーダー。
	Shader						m_ps;									//ピクセルシェーダー。
	Shader						m_pss;									//ピクセルシェーダー。
	CVector4					m_mulCol = CVector4::White;
private:
	void InitCommon(float w, float h);
	/*!
	*@brief	定数バッファの初期化。
	*/
	void InitConstantBuffer();
	void InitVertexBuffer(float w, float h);
	void InitIndexBuffer();
	void InitSamplerState();

	ID3D11BlendState* pBlendState = NULL;
};
