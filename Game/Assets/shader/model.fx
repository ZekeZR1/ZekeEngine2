/*!
 * @brief	���f���V�F�[�_�[�B
 */


 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
 //�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);
//ShadowMap
Texture2D<float4> g_shadowMap : register(t2);		
//�@���}�b�v
Texture2D<float4> normalMap : register(t3);
//�X�y�L�����}�b�v
Texture2D<float4> specularMap : register(t4);
//Texture2D<float4> specularMap : register(t4);
//�{�[���s��
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
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
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
	float4x4 mLightView;	//���C�g�r���[�s��B
	float4x4 mLightProj;	//���C�g�v���W�F�N�V�����s��B
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
	float ambientLight;
	int hasNormalMap;
	int hasSpecularMap;
};

cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//���C�g�r���[�v���W�F�N�V�����s��B
}
/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal   : NORMAL;				//�@���B
	float3 Tangent  : TANGENT;				//�ڃx�N�g���B
	float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal   : NORMAL;				//�@���B
	float2 TexCoord	: TEXCOORD0;			//UV���W�B
	float3 Tangent	: TANGENT;				//�ڃx�N�g���B
	uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
	float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};


/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
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
	float4 Position 			: SV_POSITION;	//���W�B
};
/*!
 *@brief	�X�L���s����v�Z�B
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
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
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
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */

PSInput VSMainSkin(VSInputNmTxWeights In)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
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
//�X�e�[�W�p�̃s�N�Z���V�F�[�_�[�B
float4 PSMainStage( PSInput In ) : SV_Target0
{
	//�e�N�X�`���J���[
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	float4 shadowColor = albedoColor * 0.5f;

	if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
									//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP��Ԃł̐[�x�l���v�Z�B
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
			float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//�e�������Ă���̂ŁA�����キ����
				albedoColor.xyz = shadowColor.xyz;
			}
		}
	}
	return albedoColor;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	#if 1
		//�e�N�X�`���J���[
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
		//�f�B���N�V�������C�g
	#if 1
		//�������̓g�D�[��
		float lig = 0.0f;
		lig = max(0.0f, dot(normal * -1.0f, mDirLight[0]));
		if (lig < 0.2f) {
			albedoColor.xyz = shadowColor.xyz;
		}
		if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
										//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
			float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
	
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP.z / In.posInLVP.w;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
				float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);
	
				if (zInLVP > zInShadowMap + 0.01f) {
					//�e�������Ă���̂ŁA�����キ����
					albedoColor.xyz = shadowColor.xyz;
				}
			}
		}
	#else
		//�������̓t�@�[�V�F�[�h�B
		//�X�y�L����
		float3 toEyeDir = normalize(eyePos - In.worldPos);
		float t = 1.0f - max(0.0f, dot(normal, toEyeDir));
		albedoColor += pow(t, specularPow);// * 0.5f;
	#endif
		return albedoColor;
	#else
		//�e�N�X�`���J���[
		float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
		//�f�B���N�V�������C�g�@
		float3 lig = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			lig += max(0.0f, dot(In.Normal * -1.0f, mDirLight[i])) * mColor[i];
		}
		//�X�y�L����
		float3 toEyeDir = normalize(eyePos - In.worldPos);
		//���_�̔��˃x�N�g��r
		float3 r = -toEyeDir + 2 * In.Normal * dot(toEyeDir, In.Normal);
		//���˃x�N�g���ƃf�B���N�V�������C�g�̃x�N�g���̓���
		float3 specLig;
		float t;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			t = max(0.0f, dot(r, -mDirLight[i]));
			//�X�y�L�������i��
			specLig = pow(t, specularPow) * mColor[i].xyz;
			lig += specLig;
		}
		//ambient
		lig += albedoColor.xyz  * ambientLight;
		if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
										//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
			float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
	
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP.z / In.posInLVP.w;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
				float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);
	
				if (zInLVP > zInShadowMap + 0.01f) {
					//�e�������Ă���̂ŁA�����キ����
					lig *= 0.5f;
				}
			}
		}
		float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		finalColor.xyz = albedoColor.xyz *lig;
		return finalColor;
	#endif
//---------------NYAAAAAAAAAAAAAAAAAAAAAAAAAAAAAANNN___________________________________
	////�e�N�X�`���J���[
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
	////�f�B���N�V�������C�g�@
	//float3 lig = 0.0f;
	//for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
	//	lig += max(0.0f, dot(In.Normal * -1.0f, mDirLight[i])) * mColor[i];
	//}
	////�X�y�L����
	//float3 toEyeDir = normalize(eyePos - In.worldPos);
	////���_�̔��˃x�N�g��r
	//float3 r = -toEyeDir + 2 * In.Normal * dot(toEyeDir, normal);
	////���˃x�N�g���ƃf�B���N�V�������C�g�̃x�N�g���̓���
	//float3 specLig;
	//float t;
	//for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
	//	t = max(0.0f, dot(r, -mDirLight[i]));
	//	//�X�y�L�������i��
	//	specLig = pow(t, specularPow) * mColor[i].xyz;
	//	lig += specLig;
	//}
	////ambient
	//lig += albedoColor.xyz  * ambientLight;
	////lig += albedoColor.xyz;//  *2.f;
	//if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
	//								//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
	//	float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
	//	shadowMapUV *= float2(0.5f, -0.5f);
	//	shadowMapUV += 0.5f;
	//	//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
	//	if (shadowMapUV.x < 1.0f
	//		&& shadowMapUV.x > 0.0f
	//		&& shadowMapUV.y < 1.0f
	//		&& shadowMapUV.y > 0.0f
	//		) {

	//		///LVP��Ԃł̐[�x�l���v�Z�B
	//		float zInLVP = In.posInLVP.z / In.posInLVP.w;
	//		//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
	//		float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

	//		if (zInLVP > zInShadowMap + 0.01f) {
	//			//�e�������Ă���̂ŁA�����キ����
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
	//�e�N�X�`���J���[
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	//�f�B���N�V�������C�g�@
	float3 lig = 0.0f;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		lig += max(0.0f, dot(In.Normal * -1.0f, mDirLight[i])) * mColor[i];
	}
	//�X�y�L����
	float3 toEyeDir = normalize(eyePos - In.worldPos);
	//���_�̔��˃x�N�g��r
	float3 r = -toEyeDir + 2 * In.Normal * dot(toEyeDir, In.Normal);
	//���˃x�N�g���ƃf�B���N�V�������C�g�̃x�N�g���̓���
	float3 specLig;
	float t;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		t = max(0.0f, dot(r, -mDirLight[i]));
		//�X�y�L�������i��
		specLig = pow(t, specPow) * mColor[i].xyz;
		lig += specLig;
	}
	lig += albedoColor.xyz  * ambientLight;
	if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
									//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP��Ԃł̐[�x�l���v�Z�B
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
			float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//�e�������Ă���̂ŁA�����キ����
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
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z / In.Position.w;
}