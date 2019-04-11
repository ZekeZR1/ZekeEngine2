#pragma once
#include "../RenderTarget.h"
#include "../Shader.h"

class PostEffect;

/// <summary>
/// ブルーム
/// </summary>
class Bloom
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Bloom();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Bloom();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(PostEffect& postEffect);
private:
	/// <summary>
	/// アルファブレンディングのステートを初期化する。
	/// </summary>
	void InitAlphaBlendState();
	/// <summary>
	/// 定数バッファの初期化。
	/// </summary>
	void InitConstantBuffer();
	/// <summary>
	/// シェーダーを初期化。
	/// </summary>
	void InitShader();
	/// <summary>
	/// レンダリングターゲットの初期化。
	/// </summary>
	void InitRenderTarget();
	/// <summary>
	/// サンプラステートの初期化。
	/// </summary>
	void InitSamplerState();
private:

	static const int NUM_WEIGHTS = 8;
	static const int NUM_DOWN_SAMPLING_RT = 10;
	/*!
	* @brief	ブラー用のパラメータバッファ。
	*@details
	* これを変更したら、Assets/shader/bloom.fxのCBBlurの中身も変更するように。
	*/
	struct SBlurParam {
		CVector4 offset;
		float weights[NUM_WEIGHTS];
	};
	float m_blurDispersion = 5.0f;		//ブラーの分散具合。大きくするほど強く分散する。
	bool m_isEnable = false;		//!<有効。
	RenderTarget m_luminanceRT;	//!<輝度を抽出するレンダリングターゲット。
	RenderTarget m_combineRT;		//!<ぼかし合成用のレンダリングターゲット。
	RenderTarget m_downSamplingRT[NUM_DOWN_SAMPLING_RT];	//!<ダウンサンプリング用のレンダリングターゲット。
	Shader m_vsShader;
	Shader m_psLuminance;			//!<輝度抽出用のピクセルシェーダー。
	Shader m_vsXBlur;				//!<XBlur用の頂点シェーダー。
	Shader m_vsYBlur;				//!<YBlur用の頂点シェーダー。
	Shader m_psBlur;				//!<XBlur用のピクセルシェーダー。
	Shader m_psCombine;			//!<合成用のピクセルシェーダー。
	Shader	m_copyVS;				//!<コピー用の頂点シェーダー。
	Shader	m_copyPS;				//!<コピー用のピクセルシェーダー。
	SBlurParam m_blurParam;			//!<ブラー用のパラメータ。
	CConstantBuffer m_cbBlur;
	ID3D11Buffer* m_blurParamCB = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	ID3D11BlendState* m_disableBlendState = nullptr;	//アルファブレンディングを無効にするブレンディングステート。
	ID3D11BlendState* m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
};
	