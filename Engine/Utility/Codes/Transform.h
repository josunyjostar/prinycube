#ifndef Transform_h__
#define Transform_h__

#include "Component.h"
#include "Camera.h"

#define MAT_S		1
#define MAT_R		2
#define MAT_T		4
#define MAT_RV		8
#define MAT_PM		16

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
protected:
	explicit CTransform();
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	DeclRTTI(CTransform, CComponent);
	friend CComponent;

	void SetScale(const _vec3& _vScale);
	void SetRotation(const _vec3& _vRotation);
	void SetRightVector(const _vec3& _vRight)	{ memcpy(m_matWorld.m[0], &(Magnitude(*(_vec3*)m_matWorld.m[0])*_vRight), sizeof(_vec3)); }
	void SetUpVector(const _vec3& _vUp)			{ memcpy(m_matWorld.m[1], &(Magnitude(*(_vec3*)m_matWorld.m[1])*_vUp), sizeof(_vec3)); }
	void SetLookVector(const _vec3& _vLook)		{ memcpy(m_matWorld.m[2], &(Magnitude(*(_vec3*)m_matWorld.m[2])*_vLook), sizeof(_vec3)); }
	void SetPosition(const _vec3& _vPosition)	{ memcpy(m_matWorld.m[3], &_vPosition, sizeof(_vec3)); }
	void SetScaleX(float _x)					{ memcpy(m_matWorld.m[0], &(this->GetRightVector()*_x), sizeof(_vec3)); }
	void SetScaleY(float _y)					{ memcpy(m_matWorld.m[1], &(this->GetUpVector()*_y), sizeof(_vec3)); }
	void SetScaleZ(float _z)					{ memcpy(m_matWorld.m[2], &(this->GetLookVector()*_z), sizeof(_vec3)); }
	void SetPositionX(float _x)					{ m_matWorld._41 = _x; }
	void SetPositionY(float _y)					{ m_matWorld._42 = _y; }
	void SetPositionZ(float _z)					{ m_matWorld._43 = _z; }
	void SetWorldMatrix(const _matrix& _matWorld) { m_matWorld = _matWorld; }
	void SetWorldMatrix(int _Transform, ...);

	void AddPosition(const _vec3& _vPosition)	{ memcpy(m_matWorld.m[3], &(*(_vec3*)m_matWorld.m[3]+_vPosition), sizeof(_vec3)); }
	void AddScale(const _vec3& _vScale);
	void AddScaleX(float _x)					{ memcpy(m_matWorld.m[0], &(this->GetRightVector()*(Magnitude(*(_vec3*)m_matWorld.m[0])+_x)), sizeof(_vec3)); }
	void AddScaleY(float _y)					{ memcpy(m_matWorld.m[1], &(this->GetUpVector()*(Magnitude(*(_vec3*)m_matWorld.m[1])+_y)), sizeof(_vec3)); }
	void AddScaleZ(float _z)					{ memcpy(m_matWorld.m[2], &(this->GetLookVector()*(Magnitude(*(_vec3*)m_matWorld.m[2])+_z)), sizeof(_vec3)); }
	void AddRotationX(float _x)					{ this->Rotate(_vec3(1.f, 0.f, 0.f), _x); }
	void AddRotationY(float _y)					{ this->Rotate(_vec3(0.f, 1.f, 0.f), _y); }
	void AddRotationZ(float _z)					{ this->Rotate(_vec3(0.f, 0.f, 1.f), _z); }
	void AddPositionX(float _x)					{ m_matWorld._41 += _x; }
	void AddPositionY(float _y)					{ m_matWorld._42 += _y; }
	void AddPositionZ(float _z)					{ m_matWorld._43 += _z; }

	_vec3 GetScale() const;
	_vec3 GetRotation()	const;
	_vec3 GetRightVector() const				{ return Normalize(*(_vec3*)m_matWorld.m[0]); }
	_vec3 GetUpVector() const					{ return Normalize(*(_vec3*)m_matWorld.m[1]); }
	_vec3 GetLookVector() const					{ return Normalize(*(_vec3*)m_matWorld.m[2]); }
	_vec3 GetPosition()	const					{ return *(_vec3*)m_matWorld.m[3]; }
	_vec2 GetScreenPos(const RCP_DEVICE& _pGraphicDev, int _iCX, int _iCY) const;
	_matrix GetScaleMatrix() const;
	_matrix GetInvScaleMatrix() const;
	_matrix GetRotationMatrix() const;
	_matrix GetPositionMatrix() const;
	_matrix GetWorldMatrix() const				{ return m_matWorld; }
	_matrix GetUIMatrix(const RCP_DEVICE& _pGraphicDev) const;
	_matrix GetBillboardMatrix(RCP_CAMERA _pCamera) const;

public:
	void DeviceInWorldMatrix(const RCP_DEVICE& _pGraphicDev) { _pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld); }
	void Rotate(const _vec3& _vAxis, const float& _fAngle);
	void Rotate(const _matrix& _matRot);
	void LookAt(const _vec3& _vAt, const _vec3& _vUp);

protected:
	_matrix		m_matWorld;
};

END

TYPEDEF_RCP(Engine::CTransform, TRANSFORM);

#endif // Transform_h__
