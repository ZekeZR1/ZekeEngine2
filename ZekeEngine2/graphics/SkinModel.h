#pragma once

#include "Skelton.h"

class DirectionLight;
/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	スキンモデルクラス。
*/
class SkinModel
{
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();

	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ, const char* entryPS = "PSMain", const char* entryVS = "VSMain", const wchar_t* normalMap = nullptr, const wchar_t* specularMap = nullptr);

	//void SetNormalMap(const wchar_t* path) {
	//	path;
	//}
	CMatrix GetWorldMatrix() {
		return m_worldMatrix;
	}
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	void FindVertexPosition(std::function<void(CVector3* pos)>func);

	void Draw(CMatrix viewMatrix, CMatrix projMatrix);

	void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix);

	void Draw();
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};

	void SetDirColor(CVector4 col, int index) {
		m_DirCol[index] = col;
	}

	void SetDirLight(CVector4 dir, int index) {
		m_DirLight[index] = dir;
	}

	void SetSpecularPow(float pow) {
		m_specPow = pow;
	}
	void SetShadowMap(ID3D11ShaderResourceView* srv) {
		m_shadowMapSRV = srv;
	}
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);
	//ディレクションライトの初期化
	void InitDirectionLight();

	void SetRenderMode(EnRenderMode renderMode)
	{
		m_renderMode = renderMode;
	}
public:
	void SetShadowReciever(bool flag) {
		m_isShadowReciever = flag;
	}
private:
	static const int NUM_DIRECTION_LIG = 4;

	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CVector4 mCol[NUM_DIRECTION_LIG];
		CVector4 mDir[NUM_DIRECTION_LIG];
		CVector3 eyePos;
		float specPow;
		CMatrix mLightView;		//todo ライトビュー行列。
		CMatrix mLightProj;		//todo ライトプロジェクション行列。
		int isShadowReciever;	//todo シャドウレシーバーのフラグ。
		float ambientLight;
		int hasNormalMap;
		int hasSpecularMap;
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
	ID3D11Buffer* m_cb = nullptr;					//!<定数バッファ。
	ID3D11Buffer* m_cb1 = nullptr;					//!<定数バッファ。
	Skeleton			m_skeleton;						//!<スケルトン。
	CMatrix				m_worldMatrix;					//!<ワールド行列。
	DirectX::Model* m_modelDx;						//!<DirectXTKが提供するモデルクラス。
	ID3D11SamplerState* m_samplerState = nullptr;		//!<サンプラステート。
	CVector4 m_DirLight[NUM_DIRECTION_LIG];// = { 0.707,-0.707,0.0f,0.0f };
	CVector4 m_DirCol[NUM_DIRECTION_LIG];// = { 1.0f,1.0f,1.0f,1.0f };
	float m_specPow = 10.f;
	const char* m_vsmain;
	const char* m_psmain;
	bool m_isShadowReciever = true;
	bool m_hasNormalMap = false;
	bool m_hasSpecularMap = false;
	EnRenderMode m_renderMode = enRenderMode_Invalid;	//レンダリングモード。
	ID3D11ShaderResourceView* m_shadowMapSRV = nullptr;
	//TODO : Zeke: ライト関係流石にアな感じでごちゃごちゃなので整理する。
	const CVector4 m_defCol = { 1.f,1.f,1.f,1.f };
	const CVector4 m_defDir = { 0.707,-0.707,0.0f,0.0f };
	//DirectionLight m_light;q
};

