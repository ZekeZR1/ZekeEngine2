#pragma once

#include "Skelton.h"

class DirectionLight;
/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel
{
public:
	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();

	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ, const char* entryPS = "PSMain", const char* entryVS = "VSMain", const wchar_t* normalMap = nullptr, const wchar_t* specularMap = nullptr);

	//void SetNormalMap(const wchar_t* path) {
	//	path;
	//}
	CMatrix GetWorldMatrix() {
		return m_worldMatrix;
	}
	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
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
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
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
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};

	void SetDirColor(CVector4 col,int index) {
		m_DirCol[index] = col;
	}

	void SetDirLight(CVector4 dir,int index) {
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
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);
	//�f�B���N�V�������C�g�̏�����
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

	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CVector4 mCol[NUM_DIRECTION_LIG];
		CVector4 mDir[NUM_DIRECTION_LIG];
		CVector3 eyePos;
		float specPow;
		CMatrix mLightView;		//todo ���C�g�r���[�s��B
		CMatrix mLightProj;		//todo ���C�g�v���W�F�N�V�����s��B
		int isShadowReciever;	//todo �V���h�E���V�[�o�[�̃t���O�B
		float ambientLight;
		int hasNormalMap;
		int hasSpecularMap;
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;					//!<�萔�o�b�t�@�B
	ID3D11Buffer*		m_cb1 = nullptr;					//!<�萔�o�b�t�@�B
	Skeleton			m_skeleton;						//!<�X�P���g���B
	CMatrix				m_worldMatrix;					//!<���[���h�s��B
	DirectX::Model*		m_modelDx;						//!<DirectXTK���񋟂��郂�f���N���X�B
	ID3D11SamplerState* m_samplerState = nullptr;		//!<�T���v���X�e�[�g�B
	CVector4 m_DirLight[NUM_DIRECTION_LIG];// = { 0.707,-0.707,0.0f,0.0f };
	CVector4 m_DirCol[NUM_DIRECTION_LIG];// = { 1.0f,1.0f,1.0f,1.0f };
	float m_specPow = 10.f;
	const char* m_vsmain;
	const char* m_psmain;
	bool m_isShadowReciever = true;
	bool m_hasNormalMap = false;
	bool m_hasSpecularMap = false;
	EnRenderMode m_renderMode = enRenderMode_Invalid;	//�����_�����O���[�h�B
	ID3D11ShaderResourceView* m_shadowMapSRV = nullptr;
	//DirectionLight m_light;q
};
