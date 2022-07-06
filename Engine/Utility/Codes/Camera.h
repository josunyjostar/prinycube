#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera();
	virtual ~CCamera();
	
public:
	DeclRTTI(CCamera, CGameObject);

	void SetViewMatrix(const _vec3& _vRight, const _vec3& _vUp, const _vec3& _vLook, const _vec3& _vPos);
	void SetViewMatrix(const _vec3& _vEye, const _vec3& _vAt, const _vec3& _vUp);
	void SetViewMatrix(const _matrix& _matView)	{ m_matView = _matView; D3DXMatrixInverse(&m_matWorld, NULL, &m_matView); }
	void SetViewMatrix()						{ D3DXMatrixInverse(&m_matView, NULL, &MoveMatrix(m_matWorld, m_ShakePos)); }
	void SetProjMatrixPerspectiveFov(_float _fov, _float _Aspect, _float _zn, _float _zf);
	void SetProjMatrixPerspective(_float _fWidth, _float _fHeight, _float _zn, _float _zf);
	void SetProjMatrixOrtho(_float _fWidth, _float _fHeight, _float _zn, _float _zf);
	void SetProjMatrixOrthoOffCenter(_float _fLeft, _float _fRight, _float _fBottom, _float _fTop, _float _zn, _float _zf);
	void SetProjMatrix(const _matrix& _matProj)	{ m_matProj = _matProj; D3DXMatrixInverse(&m_matInvProj, NULL, &m_matProj); }
	void SetRightVector(const _vec3& _vRight)	{ memcpy(m_matWorld.m[0], _vRight, sizeof(_vec3)); }
	void SetUpVector(const _vec3& _vUp)			{ memcpy(m_matWorld.m[1], _vUp, sizeof(_vec3)); }
	void SetLookVector(const _vec3& _vLook)		{ memcpy(m_matWorld.m[2], _vLook, sizeof(_vec3)); }
	void SetPosition(const _vec3& _vPos)		{ memcpy(m_matWorld.m[3], _vPos, sizeof(_vec3)); }
	void SetYaw(const _float& _fYaw)			{ m_fYaw = _fYaw; }
	void SetPitch(const _float& _fPitch)		{ m_fPitch = _fPitch; }
	void SetMinPitch(const _float& _fMinPitch)	{ m_fMinPitch = _fMinPitch; }
	void SetMaxPitch(const _float& _fMaxPitch)	{ m_fMaxPitch = _fMaxPitch; }
	void SetGraphicDev(RCP_DEVICE _pGraphicDev) { m_pGraphicDev = _pGraphicDev; }
	void SetTransform();

	_float GetYaw() const						{ return m_fYaw; }
	_float GetPitch() const						{ return m_fPitch; }
	_float GetMinPitch() const					{ return m_fMinPitch; }
	_float GetMaxPitch() const					{ return m_fMaxPitch; }
	_vec3 GetRightVector() const				{ return *(_vec3*)m_matWorld.m[0]; }
	_vec3 GetUpVector() const					{ return *(_vec3*)m_matWorld.m[1]; }
	_vec3 GetLookVector() const					{ return *(_vec3*)m_matWorld.m[2]; }
	_vec3 GetPosition() const					{ return *(_vec3*)m_matWorld.m[3]; }
	_matrix GetWorldMatrix() const				{ return m_matWorld; }
	_matrix GetViewMatrix() const				{ return m_matView; }
	_matrix GetProjMatrix() const				{ return m_matProj; }
	_ray GetCamRay(_ray _rViewRay);
	_ray GetCamRay(_vec2 _vMousePos);

public:
	virtual void Update(const _float& _fDeltaTime);
	void Shake(const _float& _fTime);

	void Rotate(const _vec3& _vAxis, const float& _fAngle);
	void RotateYawPitch(const _float& _fYaw, const _float& _fPitch);

protected:
	RCP_DEVICE	m_pGraphicDev;
	_matrix		m_matWorld;
	_matrix		m_matView;
	_matrix		m_matProj;
	_matrix		m_matInvProj;

	_vec3		m_ShakePos;
	_float		m_fShakeTime;
	_float		m_fRunTime;

	_float		m_fYaw;
	_float		m_fPitch;
	_float		m_fMinPitch;
	_float		m_fMaxPitch;

public:
	CREATE(CCamera);
};

END

TYPEDEF_RCP(Engine::CCamera, CAMERA);

#endif // Camera_h__
