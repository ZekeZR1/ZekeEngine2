/*!
 * @brief	モデルシェーダー。
 */


 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
 //アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);
//ShadowMap
Texture2D<float4> g_shadowMap : register(t2);		
//法線マップ
Texture2D<float4> normalMap : register(t3);
//スペキュラマップ
Texture2D<float4> specularMap : register(t4);
//Texture2D<float4> specularMap : register(t4);
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

//normal
float3 CalcNormal(float3 normal, float3 biNormal, float3 tangent, float2 uv) {
	float3 binSpaceNormal = normalMap.Sample(Sampler, uv).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
	normal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
	return normal;
}
/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
static const int NUM_DIRECTION_LIG = 4;

cbuffer VSPSCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 mColor[NUM_DIRECTION_LIG];
	float4 mDirLight[NUM_DIRECTION_LIG];
	float3 eyePos;
	float specPow;
	float4x4 mLightView;	//ライトビュー行列。
	float4x4 mLightProj;	//ライトプロジェクション行列。
	int isShadowReciever;	//シャドウレシーバーフラグ。
	float ambientLight;
	int hasNormalMap;
	int hasSpecularMap;
};

cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//ライトビュープロジェクション行列。
}
/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal   : NORMAL;				//法線。
	float3 Tangent  : TANGENT;				//接ベクトル。
	float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal   : NORMAL;				//法線。
	float2 TexCoord	: TEXCOORD0;			//UV座標。
	float3 Tangent	: TANGENT;				//接ベクトル。
	uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
	float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};


/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput {
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos	: TEXCOORD1;
	float4 posInLVP	: TEXCOORD2;
};

struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};
/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
		w += In.Weights[i];
	}

	skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain(VSInputNmTxVcTangent In)
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	float4 worldPos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));

	if (isShadowReciever == 1) {
		psInput.posInLVP = mul(mLightView, worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}

	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */

PSInput VSMainSkin(VSInputNmTxWeights In)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
			skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
			w += In.Weights[i];
		}
		skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
		pos = mul(skinning, In.Position);
	}
	psInput.Normal = normalize(mul(skinning, In.Normal));
	psInput.Tangent = normalize(mul(skinning, In.Tangent));
	float4 worldPos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;

	if (isShadowReciever == 1) {
		psInput.posInLVP = mul(mLightView, worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}

	return psInput;
}
//ステージ用のピクセルシェーダー。
float4 PSMainStage( PSInput In ) : SV_Target0
{
	//テクスチャカラー
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	float4 shadowColor = albedoColor * 0.5f;

	if (isShadowReciever == 1) {	//シャドウレシーバー。
									//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//シャドウマップの範囲内かどうかを判定する。
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP空間での深度値を計算。
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//シャドウマップに書き込まれている深度値を取得。
			float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//影が落ちているので、光を弱くする
				albedoColor.xyz = shadowColor.xyz;
			}
		}
	}
	return albedoColor;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	#if 1
		//テクスチャカラー
		float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
		//float4 albedoColor = normalMap.Sample(Sampler, In.TexCoord);
		float3 biNormal = normalize(cross(In.Tangent, In.Normal));
		float3 normal;
		if (hasNormalMap == 1) {
			normal = CalcNormal(In.Normal, biNormal, In.Tangent, In.TexCoord);
		}
		else {
			normal = In.Normal;
		}
		float specularPow = 0.f;
		if (hasSpecularMap == 1) {
			float4 specular = specularMap.Sample(Sampler, In.TexCoord);
			specularPow = specular.x;
		}
		else {
			specularPow = specPow;
		}
		float4 shadowColor = albedoColor * 0.5f;
		//ディレクションライト
	#if 1
		//こっちはトゥーン
		float lig = 0.0f;
		lig = max(0.0f, dot(normal * -1.0f, mDirLight[0]));
		if (lig < 0.2f) {
			albedoColor.xyz = shadowColor.xyz;
		}
		if (isShadowReciever == 1) {	//シャドウレシーバー。
										//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
			float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//シャドウマップの範囲内かどうかを判定する。
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
	
				///LVP空間での深度値を計算。
				float zInLVP = In.posInLVP.z / In.posInLVP.w;
				//シャドウマップに書き込まれている深度値を取得。
				float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);
	
				if (zInLVP > zInShadowMap + 0.01f) {
					//影が落ちているので、光を弱くする
					albedoColor.xyz = shadowColor.xyz;
				}
			}
		}
	#else
		//こっちはファーシェード。
		//スペキュラ
		float3 toEyeDir = normalize(eyePos - In.worldPos);
		float t = 1.0f - max(0.0f, dot(normal, toEyeDir));
		albedoColor += pow(t, specularPow);// * 0.5f;
	#endif
		return albedoColor;
	#else
		//テクスチャカラー
		float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
		//ディレクションライト　
		float3 lig = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			lig += max(0.0f, dot(In.Normal * -1.0f, mDirLight[i])) * mColor[i];
		}
		//スペキュラ
		float3 toEyeDir = normalize(eyePos - In.worldPos);
		//視点の反射ベクトルr
		float3 r = -toEyeDir + 2 * In.Normal * dot(toEyeDir, In.Normal);
		//反射ベクトルとディレクションライトのベクトルの内積
		float3 specLig;
		float t;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			t = max(0.0f, dot(r, -mDirLight[i]));
			//スペキュラを絞る
			specLig = pow(t, specularPow) * mColor[i].xyz;
			lig += specLig;
		}
		//ambient
		lig += albedoColor.xyz  * ambientLight;
		if (isShadowReciever == 1) {	//シャドウレシーバー。
										//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
			float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//シャドウマップの範囲内かどうかを判定する。
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
	
				///LVP空間での深度値を計算。
				float zInLVP = In.posInLVP.z / In.posInLVP.w;
				//シャドウマップに書き込まれている深度値を取得。
				float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);
	
				if (zInLVP > zInShadowMap + 0.01f) {
					//影が落ちているので、光を弱くする
					lig *= 0.5f;
				}
			}
		}
		float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		finalColor.xyz = albedoColor.xyz *lig;
		return finalColor;
	#endif
//---------------NYAAAAAAAAAAAAAAAAAAAAAAAAAAAAAANNN___________________________________
	////テクスチャカラー
	//float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	//float3 biNormal = normalize(cross(In.Tangent, In.Normal));
	//float3 normal;
	//if (hasNormalMap == 1) {
	//	normal = CalcNormal(In.Normal, biNormal, In.Tangent, In.TexCoord);
	//}
	//else {
	//	normal = In.Normal;
	//}
	//float specularPow = 0.f;
	//if (hasSpecularMap == 1) {
	//	float4 specular = specularMap.Sample(Sampler, In.TexCoord);
	//	specularPow = specular.r;
	//}
	//else {
	//	specularPow = specPow;
	//}
	//float4 shadowColor = albedoColor * 0.5f;
	////ディレクションライト　
	//float3 lig = 0.0f;
	//for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
	//	lig += max(0.0f, dot(In.Normal * -1.0f, mDirLight[i])) * mColor[i];
	//}
	////スペキュラ
	//float3 toEyeDir = normalize(eyePos - In.worldPos);
	////視点の反射ベクトルr
	//float3 r = -toEyeDir + 2 * In.Normal * dot(toEyeDir, normal);
	////反射ベクトルとディレクションライトのベクトルの内積
	//float3 specLig;
	//float t;
	//for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
	//	t = max(0.0f, dot(r, -mDirLight[i]));
	//	//スペキュラを絞る
	//	specLig = pow(t, specularPow) * mColor[i].xyz;
	//	lig += specLig;
	//}
	////ambient
	//lig += albedoColor.xyz  * ambientLight;
	////lig += albedoColor.xyz;//  *2.f;
	//if (isShadowReciever == 1) {	//シャドウレシーバー。
	//								//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
	//	float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
	//	shadowMapUV *= float2(0.5f, -0.5f);
	//	shadowMapUV += 0.5f;
	//	//シャドウマップの範囲内かどうかを判定する。
	//	if (shadowMapUV.x < 1.0f
	//		&& shadowMapUV.x > 0.0f
	//		&& shadowMapUV.y < 1.0f
	//		&& shadowMapUV.y > 0.0f
	//		) {

	//		///LVP空間での深度値を計算。
	//		float zInLVP = In.posInLVP.z / In.posInLVP.w;
	//		//シャドウマップに書き込まれている深度値を取得。
	//		float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

	//		if (zInLVP > zInShadowMap + 0.01f) {
	//			//影が落ちているので、光を弱くする
	//			lig *= 0.5f;
	//		}
	//	}
	//}
	//float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//finalColor.xyz = albedoColor.xyz *lig;
	//return finalColor; 
}

float4 PSMainSkin(PSInput In) : SV_Target0
{
	//テクスチャカラー
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	//ディレクションライト　
	float3 lig = 0.0f;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		lig += max(0.0f, dot(In.Normal * -1.0f, mDirLight[i])) * mColor[i];
	}
	//スペキュラ
	float3 toEyeDir = normalize(eyePos - In.worldPos);
	//視点の反射ベクトルr
	float3 r = -toEyeDir + 2 * In.Normal * dot(toEyeDir, In.Normal);
	//反射ベクトルとディレクションライトのベクトルの内積
	float3 specLig;
	float t;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		t = max(0.0f, dot(r, -mDirLight[i]));
		//スペキュラを絞る
		specLig = pow(t, specPow) * mColor[i].xyz;
		lig += specLig;
	}
	lig += albedoColor.xyz  * ambientLight;
	if (isShadowReciever == 1) {	//シャドウレシーバー。
									//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//シャドウマップの範囲内かどうかを判定する。
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP空間での深度値を計算。
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//シャドウマップに書き込まれている深度値を取得。
			float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//影が落ちているので、光を弱くする
				lig *= 0.5f;
			}
		}
	}
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz *lig;
	return finalColor;
}

////none bone
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	//float4 pos = 0;
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4 worldPos = mul(mWorld, In.Position);
	worldPos = mul(mView, worldPos);
	worldPos = mul(mProj, worldPos);
	psInput.Position = worldPos;
	return psInput;
}

PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;
	float4 pos = 0;
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
			w += In.Weights[i];
		}
		skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
		pos = mul(skinning, In.Position);
	}
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4 worldPos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}

float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}