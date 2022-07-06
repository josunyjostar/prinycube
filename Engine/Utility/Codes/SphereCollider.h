#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Collider.h"

BEGIN(Engine)

class CCollisionManager;
class ENGINE_DLL CSphereCollider : public CCollider
{
private:
	explicit CSphereCollider():m_fRadius(1.f) {m_ColType = COL_SPHERE;}
	virtual ~CSphereCollider() {}

public:
	DeclRTTI(CSphereCollider, CCollider);
	friend CComponent;
	friend CCollisionManager;

	void	SetPosition(const _vec3& _vPos)		{ m_vPosition = _vPos; }
	void	SetRadius(const _float& _fRadius)	{ m_fRadius = _fRadius; }

	_vec3	GetPosition() const					{ return m_vPosition; }
	_float	GetRadius() const 					{ return m_fRadius; }

private:
	_vec3	m_vPosition;
	_float	m_fRadius;
};

END

TYPEDEF_RCP(Engine::CSphereCollider, SPHERE);

#endif // SphereCollider_h__
