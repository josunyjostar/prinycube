#ifndef RayCast_h__
#define RayCast_h__

#include "CollisionManager.h"

BEGIN(Engine)

class ENGINE_DLL CRayCast : public TSingleton<CRayCast>
{
private:
	explicit CRayCast();
	virtual ~CRayCast();

public:
	friend TSingleton;

	void Init(CCollisionManager* _pCollisionMgr) { m_pCollisionMgr = _pCollisionMgr; }
	COL_INFO Picking(const _ray& _rRay, const _char* _pLayerTag);

private:
	CCollisionManager* m_pCollisionMgr;

private:
	bool CollisionCheck(CSphereCollider* _pSphere, const _ray& _rRay, _vec3& _vColPos);
	bool CollisionCheck(CCubeCollider* _pCube, const _ray& _rRay, _vec3& _vColPos);
};

extern CRayCast* const RayCast;

END

#endif // RayCast_h__
