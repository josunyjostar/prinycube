#ifndef CubeCollider_h__
#define CubeCollider_h__

#include "Collider.h"

BEGIN(Engine)

class CCollisionManager;
class ENGINE_DLL CCubeCollider : public CCollider
{
private:
	explicit CCubeCollider():m_vSize(1.f, 1.f, 1.f) {m_ColType = COL_CUBE;};
	virtual ~CCubeCollider() {};

public:
	DeclRTTI(CCubeCollider, CCollider);
	friend CComponent;
	friend CCollisionManager;

	void	SetPosition(const _vec3& _vPos)	{ m_vPosition = _vPos; }
	void	SetSize(_vec3 _size)			{ m_vSize = _size;}

	_vec3	GetPosition() const				{ return m_vPosition; }
	_vec3	GetSize() const					{ return m_vSize; }

private:
	_vec3	m_vPosition;
	_vec3	m_vSize;
};

END

TYPEDEF_RCP(Engine::CCubeCollider, CUBE);

#endif // CubeCollider_h__
