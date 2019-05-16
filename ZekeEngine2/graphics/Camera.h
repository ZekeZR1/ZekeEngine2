#pragma once
class Camera
{
public:
	Camera();
	~Camera();
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<�����ˉe�s��
		enUpdateProjMatrixFunc_Ortho,			//!<���s���e
	};
	virtual void Update();

	void SetPosition( const CVector3& pos ) 
	{
		m_position = pos;
	}
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	void SetTarget( const CVector3& target )
	{
		m_target = target;
	}
	const CVector3& GetTarget() const
	{
		return m_target;
	}
	void SetUp( const CVector3& up )
	{
		m_up = up;
	}
	const CVector3& GetUp() const
	{
		return m_up;
	}
	const CMatrix& GetViewMatrixInv() const
	{
		return m_viewMatrixInv;
	}
	const CMatrix& GetViewMatrix() const
	{
		return m_viewMatrix;
	}
	const CMatrix& GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}
	const CMatrix& GetViewProjectionMatrix() const
	{
		return m_viewProjectionMatrix;
	}
	const CMatrix& GetCameraRotation() const
	{
		return m_cameraRotation;
	}
	void SetFar( float fFar )
	{
		m_far = fFar;
	}
	void SetNear( float fNear )
	{
		m_near = fNear;
	}
	float GetFar() const
	{
		return m_far;
	}
	float GetNear() const
	{
		return m_near;
	}
	void SetWidth(float w)
	{
		m_width = w;
	}
	/*!
	* @brief	���s���e�̍�����ݒ�B
	*@details
	* m_updateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho�̎��Ɏg�p�����B
	*/
	void SetHeight(float h)
	{
		m_height = h;
	}
	/*!
	* @brief	���s���e�̍�����ݒ�B
	*@details
	* m_updateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho�̎��Ɏg�p�����B
	*/
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjMatrixFunc = func;
	}
	/*!
	* @brief	��p��ݒ�B
	* @details
	*  m_updateProjMatrixFunc��enUpdateProjMatrixFunc_Perspective�̎��Ɏg�p�����B
	*/
	void SetViewAngle(float viewAngle)
	{
		m_viewAngle = viewAngle;
	}
	/*!
	* @brief	��p���擾�B
	*/
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/*!
	* @brief	�����_�Ǝ��_�̋������擾�B
	*/
	float GetTargetToPositionLength() const
	{
		return m_targetToPositionLen;
	}
	/*!
	* @brief	�J�����̑O���������擾�B
	*/
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/*!
	* @brief	�J�����̉E�������擾�B
	*/
	const CVector3& GetRight() const
	{
		return m_right;
	}
	void CalcScreenPositionFromWorldPosition(CVector2& screenPos, const CVector3& worldPos) const;
protected:
	float		m_targetToPositionLen = 1.0f;		//!<�����_�Ǝ��_�܂ŋ����B
	CVector3	m_position;							//!<�J�����ʒu�B
	CVector3	m_up;								//!<�J�����̏�����B
	CVector3	m_target;							//!<�J�����̒��~�_�B
	CMatrix		m_viewMatrix;						//!<�r���[�s��B
	CMatrix		m_projectionMatrix;					//!<�v���W�F�N�V�����s��B
	CMatrix		m_viewProjectionMatrix;				//!<�r���[�v���W�F�N�V�����s��B
	CMatrix		m_viewMatrixInv;					//!<�r���[�s��̋t�s��B
	CMatrix		m_cameraRotation;					//!<�J�����̉�]�s��B

	CVector3	m_forward = CVector3::Front();		//!<�J�����̑O���B
	CVector3	m_right = CVector3::Right();			//!<�J�����̉E�B
	float		m_near;								//!<�ߕ��ʁB
	float		m_far;								//!<�����ʁB
	float		m_viewAngle;						//!<��p(���W�A��)�B
	float		m_aspect;							//!<�A�X�y�N�g��B
	float		m_width = 1280.0f;
	float		m_height = 720.0f;
	EnUpdateProjMatrixFunc m_updateProjMatrixFunc = enUpdateProjMatrixFunc_Perspective;	//!<�v���W�F�N�V�����s��̍X�V�̎d���B
	bool		m_isNeedUpdateProjectionMatrix;
};


