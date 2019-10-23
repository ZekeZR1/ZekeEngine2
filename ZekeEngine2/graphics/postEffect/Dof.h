//#pragma once
//
//#include "GaussianBlur.h"
//
//class PostEffect;
//
//class Dof
//{
//public:
//	/// <summary>
//	/// デストラクタ。
//	/// </summary>
//	~Dof();
//	/// <summary>
//	/// 初期化。
//	/// </summary>
//	void Init();
//	/// <summary>
//	/// 描画。
//	/// </summary>
//	/// <param name="postEffect"></param>
//	void Draw(PostEffect& postEffect);
//private:
//	GaussianBlur m_bokeBlur;	//ボケテクスチャをぼかすためのガウシアンブラー。
//	ID3D11BlendState* m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
//
//	Shader m_vs;				//頂点シェーダー。
//	Shader m_psFinal;			//最終ピクセルシェーダー。
//};
//
