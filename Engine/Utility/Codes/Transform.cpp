#include "Transform.h"
using namespace Engine;

CTransform::CTransform()
{
}

CTransform::~CTransform()
{
}

void CTransform::SetScale(const _vec3& _vScale)
{
	memcpy(m_matWorld.m[0], &(this->GetRightVector()*_vScale.x), sizeof(_vec3));
	memcpy(m_matWorld.m[1], &(this->GetUpVector()*_vScale.y), sizeof(_vec3));
	memcpy(m_matWorld.m[2], &(this->GetLookVector()*_vScale.z), sizeof(_vec3));
}

void CTransform::SetRotation(const _vec3& _vRotation)
{
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, _vRotation.y, _vRotation.x, _vRotation.z);

	_vec3 vScale = this->GetScale();
	memcpy(m_matWorld.m[0], &(*(_vec3*)matRot.m[0]*vScale.x), sizeof(_vec3));
	memcpy(m_matWorld.m[1], &(*(_vec3*)matRot.m[1]*vScale.y), sizeof(_vec3));
	memcpy(m_matWorld.m[2], &(*(_vec3*)matRot.m[2]*vScale.z), sizeof(_vec3));
}

void CTransform::AddScale(const _vec3& _vScale)
{
	memcpy(m_matWorld.m[0], &(this->GetRightVector()*(Magnitude(*(_vec3*)m_matWorld.m[0])+_vScale.x)), sizeof(_vec3));
	memcpy(m_matWorld.m[1], &(this->GetUpVector()*(Magnitude(*(_vec3*)m_matWorld.m[1])+_vScale.y)), sizeof(_vec3));
	memcpy(m_matWorld.m[2], &(this->GetLookVector()*(Magnitude(*(_vec3*)m_matWorld.m[2])+_vScale.z)), sizeof(_vec3));
}

void CTransform::SetWorldMatrix(int _Transform, ...)
{
	va_list ap;
	va_start(ap, _Transform);

	D3DXMatrixIdentity(&m_matWorld);

	if(_Transform & MAT_S)
	{
		_vec3 vScale = va_arg(ap, _vec3);
		m_matWorld._11 = vScale.x;
		m_matWorld._22 = vScale.y;
		m_matWorld._33 = vScale.z;
	}
	if(_Transform & MAT_R)
	{
		_vec3 vRotation = va_arg(ap, _vec3);
		_matrix matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, vRotation.y, vRotation.x, vRotation.z);
		m_matWorld *= matRot;		
	}
	if(_Transform & MAT_T)
	{
		_vec3 vPosition = va_arg(ap, _vec3);
		memcpy(m_matWorld.m[3], &vPosition, sizeof(_vec3));
	}
	if(_Transform & MAT_RV)
	{
		_vec3 vRotation = va_arg(ap, _vec3);
		_matrix matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, vRotation.y, vRotation.x, vRotation.z);
		m_matWorld *= matRot;
	}
	if(_Transform & MAT_PM)
	{
		_matrix matParents = va_arg(ap, _matrix);
		m_matWorld *= matParents;
	}

	va_end(ap);
}

_vec3 CTransform::GetScale() const
{
	return _vec3(Magnitude((*(_vec3*)m_matWorld.m[0])),
		Magnitude((*(_vec3*)m_matWorld.m[1])),
		Magnitude((*(_vec3*)m_matWorld.m[2])));
}

_vec3 CTransform::GetRotation()	const
{
	_vec3 RightVector = GetRightVector();
	return _vec3(asinf(this->GetUpVector().z), -asinf(RightVector.z), asinf(RightVector.y)) ;
}

_vec2 CTransform::GetScreenPos(const RCP_DEVICE& _pGraphicDev, int _iCX, int _iCY) const
{
	_vec3 vPos = this->GetPosition();
	_matrix matView, matProj;

	_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	vPos = TransformCoord(vPos, matView);
	vPos = TransformCoord(vPos, matProj);

	return _vec2((1.f + vPos.x) * _iCX / 2.f, (1.f - vPos.y) * _iCY / 2.f);
}

_matrix CTransform::GetScaleMatrix() const
{
	return _matrix(Magnitude((*(_vec3*)m_matWorld.m[0])),0,0,0, 
		0,Magnitude((*(_vec3*)m_matWorld.m[1])),0,0, 
		0,0,Magnitude((*(_vec3*)m_matWorld.m[2])),0, 
		0,0,0,1);
}

_matrix CTransform::GetInvScaleMatrix() const
{
	return _matrix(1.f/Magnitude((*(_vec3*)m_matWorld.m[0])),0,0,0, 
		0,1.f/Magnitude((*(_vec3*)m_matWorld.m[1])),0,0, 
		0,0,1.f/Magnitude((*(_vec3*)m_matWorld.m[2])),0, 
		0,0,0,1);
}

_matrix CTransform::GetRotationMatrix() const
{
	_matrix matRot;
	for(int i = 0; i < 3; ++i)
		memcpy(matRot.m[i], &Normalize(*(_vec3*)m_matWorld.m[i]), sizeof(_vec3));
	return matRot;
}

_matrix CTransform::GetPositionMatrix() const
{
	return _matrix(1,0,0,0, 0,1,0,0, 0,0,1,0, m_matWorld._41,m_matWorld._42,m_matWorld._43,1);
}

_matrix CTransform::GetUIMatrix(const RCP_DEVICE& _pGraphicDev) const
{
	_matrix matWorld, matView, matProj;

	matWorld = m_matWorld;
	_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	float x = 1.f / (matProj._11 * 0.5f);
	float y = 1.f / (matProj._22 * 0.5f);

	memcpy(matWorld.m[0], &(*(_vec3*)m_matWorld.m[0] * x), sizeof(_vec3));
	memcpy(matWorld.m[1], &(*(_vec3*)m_matWorld.m[1] * y), sizeof(_vec3));

	matWorld._41 *= x;
	matWorld._42 *= y;

	D3DXMatrixInverse(&matView, NULL, &matView);

	return matWorld * matView;
}

_matrix CTransform::GetBillboardMatrix(RCP_CAMERA _pCamera) const
{
	_vec3 vScale = this->GetScale();
	_matrix matBillboard = _matrix(vScale.x,0,0,0, 0,vScale.y,0,0, 0,0,vScale.z,0, 0,0,0,1) * _pCamera->GetWorldMatrix();
	memcpy(matBillboard.m[3], m_matWorld.m[3], sizeof(_vec3));
	return matBillboard;
}

void CTransform::Rotate(const _vec3& _vAxis, const float& _fAngle)
{
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &Normalize(_vAxis), _fAngle);

	for(int i = 0; i < 3; ++i)
		D3DXVec3TransformNormal((_vec3*)m_matWorld.m[i], (_vec3*)m_matWorld.m[i], &matRot);
}

void CTransform::Rotate(const _matrix& _matRot)
{
	for(int i = 0; i < 3; ++i)
		D3DXVec3TransformNormal((_vec3*)m_matWorld.m[i], (_vec3*)m_matWorld.m[i], &_matRot);
}

void CTransform::LookAt(const _vec3& _vAt, const _vec3& _vUp)
{
	_vec3 vLook = Normalize(_vAt);
	_vec3 vUp = Normalize(_vUp);
	_vec3 vRight = Cross(vUp, vLook);
	vUp = Cross(vLook, vRight);

	this->SetLookVector(vLook);
	this->SetUpVector(vUp);
	this->SetRightVector(vRight);
}