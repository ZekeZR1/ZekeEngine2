#include "stdafx.h"
#include "Camera.h"

Camera* camera3d = NULL;
Camera* camera2d = NULL;

Camera::Camera() : 
	m_near(1.1f),
	m_far(1000.0f),
	m_viewAngle(CMath::DegToRad(60.0f)),
	m_aspect(0.0f),
	m_isNeedUpdateProjectionMatrix(true),
	m_position(CVector3::Zero()),
	m_up(CVector3::Up()),
	m_target(CVector3::Zero()),
	m_viewMatrix(CMatrix::Identity()),
	m_projectionMatrix(CMatrix::Identity()),
	m_viewProjectionMatrix(CMatrix::Identity()),
	m_viewMatrixInv(CMatrix::Identity())
{

}


Camera::~Camera()
{

}

void Camera::Update(){
	if (m_isNeedUpdateProjectionMatrix) {
		if (m_updateProjMatrixFunc == enUpdateProjMatrixFunc_Perspective) {
			float aspect = FRAME_BUFFER_W / FRAME_BUFFER_H;
			m_projectionMatrix.MakeProjectionMatrix(
				m_viewAngle,
				aspect,
				m_near,
				m_far
			);
		}
		else {
			m_projectionMatrix.MakeOrthoProjectionMatrix(m_width, m_height, m_near, m_far);
		}
	}
	m_viewMatrix.MakeLookAt(m_position, m_target, m_up);
	m_viewProjectionMatrix.Mul(m_viewMatrix, m_projectionMatrix);
	m_viewMatrixInv.Inverse(m_viewMatrix);
	m_forward.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	m_right.Set(m_viewMatrixInv.m[0][0], m_viewMatrixInv.m[0][1], m_viewMatrixInv.m[0][2]);
	m_cameraRotation = m_viewMatrixInv;
	m_cameraRotation.m[3][0] = 0.0f;
	m_cameraRotation.m[3][1] = 0.0f;
	m_cameraRotation.m[3][2] = 0.0f;
	m_cameraRotation.m[3][3] = 1.0f;

	CVector3 toPos;
	toPos.Subtract(m_position, m_target);
	m_targetToPositionLen = toPos.Length();
}


void Camera::CalcScreenPositionFromWorldPosition(CVector2& screenPos, const CVector3& worldPos) const
{
	float half_w = FRAME_BUFFER_W * 0.5f;
	float half_h = FRAME_BUFFER_H * 0.5f;
	CVector4 _screenPos;
	_screenPos.Set(worldPos.x, worldPos.y, worldPos.z, 1.0f);
	m_viewProjectionMatrix.Mul(_screenPos);
	screenPos.x = (_screenPos.x / _screenPos.w)*half_w;
	screenPos.y = (_screenPos.y / _screenPos.w)*half_h;
}