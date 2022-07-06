#include "Camera.h"
using namespace Engine;

CCamera::CCamera()
:m_fYaw(0.f)
,m_fPitch(0.f)
,m_fMinPitch(-PI/2.f+0.1f)
,m_fMaxPitch(PI/2.f-0.1f)
{
}

CCamera::~CCamera()
{
	m_pGraphicDev = NULL;
}

void CCamera::SetViewMatrix(const _vec3& _vRight, const _vec3& _vUp, const _vec3& _vLook, const _vec3& _vPos)
{
	D3DXMatrixIdentity(&m_matWorld);

	memcpy(m_matWorld.m[0], _vRight, sizeof(_vec3));
	memcpy(m_matWorld.m[1], _vUp, sizeof(_vec3));
	memcpy(m_matWorld.m[2], _vLook, sizeof(_vec3));

	D3DXMatrixTranspose(&m_matView, &m_matWorld);

	memcpy(m_matWorld.m[3], _vPos+m_ShakePos, sizeof(_vec3));
	memcpy(m_matView.m[3], &-TransformNormal(_vPos+m_ShakePos, m_matView), sizeof(_vec3));
}

void CCamera::SetViewMatrix(const _vec3& _vEye, const _vec3& _vAt, const _vec3& _vUp)
{
	_vec3 vLook		= Normalize(_vAt - _vEye);
	_vec3 vRight	= Cross(Normalize(_vUp), vLook);
	_vec3 vUp		= Cross(vLook, vRight);

	this->SetViewMatrix(vRight, vUp, vLook, _vEye);
}

void CCamera::SetProjMatrixPerspectiveFov(_float _fov, _float _Aspect, _float _zn, _float _zf)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, _fov, _Aspect, _zn, _zf);
	D3DXMatrixInverse(&m_matInvProj, NULL, &m_matProj);
}

void CCamera::SetProjMatrixPerspective(_float _fWidth, _float _fHeight, _float _zn, _float _zf)
{
	D3DXMatrixPerspectiveLH(&m_matProj, _fWidth, _fHeight, _zn, _zf);
	D3DXMatrixInverse(&m_matInvProj, NULL, &m_matProj);
}

void CCamera::SetProjMatrixOrtho(_float _fWidth, _float _fHeight, _float _zn, _float _zf)
{
	D3DXMatrixOrthoLH(&m_matProj, _fWidth, _fHeight, _zn, _zf);
	D3DXMatrixInverse(&m_matInvProj, NULL, &m_matProj);
}

void CCamera::SetProjMatrixOrthoOffCenter(_float _fLeft, _float _fRight, _float _fBottom, _float _fTop, _float _zn, _float _zf)
{
	D3DXMatrixOrthoOffCenterLH(&m_matProj, _fLeft, _fRight, _fBottom, _fTop, _zn, _zf);
	D3DXMatrixInverse(&m_matInvProj, NULL, &m_matProj);
}

void CCamera::SetTransform() 
{
	SetViewMatrix();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

_ray CCamera::GetCamRay(_ray _rViewRay)
{
	_rViewRay.Dir = TransformNormal(_rViewRay.Dir, m_matWorld);
	_rViewRay.Dir = Normalize(_rViewRay.Dir);
	_rViewRay.Pos = this->GetPosition();
	
	return _rViewRay;
}

_ray CCamera::GetCamRay(_vec2 _vMousePos)
{
	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_ray ray;

	//직교 투영
	ray.Dir = this->GetLookVector();
	ray.Pos = this->GetPosition() + 
		this->GetRightVector() * ((_vMousePos.x / ViewPort.Width * 2.f - 1.f) * (1.f / (m_matProj._11))) +
		this->GetUpVector() * ((1.f - _vMousePos.y / ViewPort.Height * 2.f) * (1.f / (m_matProj._22)));
	/*
	//원근 투영
	ray.Dir.x = _vMousePos.x / ViewPort.Width * 2.f - 1.f;
	ray.Dir.y = 1.f - _vMousePos.y / ViewPort.Height * 2.f;
	ray.Dir = TransformCoord(ray.Dir, m_matInvProj);
	ray.Dir = TransformNormal(ray.Dir, m_matWorld);
	ray.Dir = Normalize(ray.Dir);
	ray.Pos = this->GetPosition();
	*/
	return ray;
}

void CCamera::Update(const _float& _fDeltaTime)
{
	if(m_fShakeTime > m_fRunTime) {
		m_fRunTime += _fDeltaTime;

		float p = pow(m_fShakeTime+10, 5) / m_fRunTime;
		m_ShakePos.x = sinf(p*1.3f)*0.1f;
		m_ShakePos.y = cosf(p)*0.12f;
		m_ShakePos.z = sinf(p*0.5f)*0.1f;
	}
	else {
		m_ShakePos = _vec3(0.f, 0.f, 0.f);
	}
}

void CCamera::Shake(const _float& _fTime)
{
	int randTime = rand()%10;
	m_fShakeTime = _fTime+randTime;
	m_fRunTime = (float)randTime;
}

void CCamera::Rotate(const _vec3& _vAxis, const float& _fAngle)
{
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &Normalize(_vAxis), _fAngle);
	m_matWorld*=matRot;
}

void CCamera::RotateYawPitch(const _float& _fYaw, const _float& _fPitch)
{
	m_fYaw+=_fYaw;
	if((m_fYaw > PI*2.f) || (m_fYaw < PI*2.f)) m_fYaw -= int(m_fYaw / (PI*2.f)) * (PI*2.f);

	m_fPitch+=_fPitch;
	if(m_fPitch > m_fMaxPitch) m_fPitch = m_fMaxPitch;
	else if(m_fPitch < m_fMinPitch) m_fPitch = m_fMinPitch;

	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, m_fYaw, m_fPitch, 0.f);
	for(int i = 0; i < 3; ++i)
		memcpy(m_matWorld.m[i], matRot.m[i], sizeof(_vec3));
}