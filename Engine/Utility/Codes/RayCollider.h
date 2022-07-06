#ifndef RayCollider_h__
#define RayCollider_h__

#include "Collider.h"

BEGIN(Engine)

class CCollisionManager;
class ENGINE_DLL CRayCollider : public CCollider
{
private:
	explicit CRayCollider():m_vDirection(0,0,1) {m_ColType = COL_RAY;}
	virtual ~CRayCollider() {}

public:
	DeclRTTI(CRayCollider, CCollider);
	friend CComponent;
	friend CCollisionManager;

	void	SetPosition(const _vec3& _vPos)		{ m_vPosition = _vPos; }
	void	SetDirection(const _vec3& _vDir)	{ m_vDirection = _vDir; }

	_vec3	GetPosition() const					{ return m_vPosition; }
	_vec3	GetDirection() const 				{ return m_vDirection; }

private:
	_vec3	m_vPosition;
	_vec3	m_vDirection;
};

END

TYPEDEF_RCP(Engine::CRayCollider, RAY);

#endif // RayCollider_h__
