#pragma once
class Camera
{
public:
	Camera();
	~Camera();
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<透視射影行列
		enUpdateProjMatrixFunc_Ortho,			//!<平行投影
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
	* @brief	平行投影の高さを設定。
	*@details
	* m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
	*/
	void SetHeight(float h)
	{
		m_height = h;
	}
	/*!
	* @brief	平行投影の高さを設定。
	*@details
	* m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
	*/
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjMatrixFunc = func;
	}
	/*!
	* @brief	画角を設定。
	* @details
	*  m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Perspectiveの時に使用される。
	*/
	void SetViewAngle(float viewAngle)
	{
		m_viewAngle = viewAngle;
	}
	/*!
	* @brief	画角を取得。
	*/
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/*!
	* @brief	注視点と視点の距離を取得。
	*/
	float GetTargetToPositionLength() const
	{
		return m_targetToPositionLen;
	}
	/*!
	* @brief	カメラの前方方向を取得。
	*/
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/*!
	* @brief	カメラの右方向を取得。
	*/
	const CVector3& GetRight() const
	{
		return m_right;
	}
	void CalcScreenPositionFromWorldPosition(CVector2& screenPos, const CVector3& worldPos) const;
protected:
	float		m_targetToPositionLen = 1.0f;		//!<注視点と視点まで距離。
	CVector3	m_position;							//!<カメラ位置。
	CVector3	m_up;								//!<カメラの上方向。
	CVector3	m_target;							//!<カメラの中止点。
	CMatrix		m_viewMatrix;						//!<ビュー行列。
	CMatrix		m_projectionMatrix;					//!<プロジェクション行列。
	CMatrix		m_viewProjectionMatrix;				//!<ビュープロジェクション行列。
	CMatrix		m_viewMatrixInv;					//!<ビュー行列の逆行列。
	CMatrix		m_cameraRotation;					//!<カメラの回転行列。

	CVector3	m_forward = CVector3::Front();		//!<カメラの前方。
	CVector3	m_right = CVector3::Right();			//!<カメラの右。
	float		m_near;								//!<近平面。
	float		m_far;								//!<遠平面。
	float		m_viewAngle;						//!<画角(ラジアン)。
	float		m_aspect;							//!<アスペクト比。
	float		m_width = 1280.0f;
	float		m_height = 720.0f;
	EnUpdateProjMatrixFunc m_updateProjMatrixFunc = enUpdateProjMatrixFunc_Perspective;	//!<プロジェクション行列の更新の仕方。
	bool		m_isNeedUpdateProjectionMatrix;
};


