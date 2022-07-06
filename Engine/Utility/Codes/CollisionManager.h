#ifndef CollisionManager_h__
#define CollisionManager_h__

#include "SphereCollider.h"
#include "CubeCollider.h"
#include "RayCollider.h"
#include "TemplateSingleton.h"

#include <map>
#include <list>
using namespace std;

BEGIN(Engine)

typedef list<CCollider*>	COL_LIST;
class ENGINE_DLL CCollisionManager : public TSingleton<CCollisionManager>
{
private:
	explicit CCollisionManager();
	virtual ~CCollisionManager();

public:
	friend TSingleton;

	COL_LIST* GetColList(const _char* _pLayerTag) { return &m_mCollisionList[_pLayerTag]; }

public:
	void	LayerCollisionCheck(const _char* _pLayerTag1, const _char* _pLayerTag2);
	bool	ColliderCollisionCheck(CCollider* _pCol, CCollider* _pCol2, _vec3& _vColPos);
	void	CallCollision(const COL_LIST::iterator& _pCol, const COL_LIST::iterator& _pCol2, _vec3& _vColPos, COL_STATE _eColState);
	void	CallTrigger(const COL_LIST::iterator& _pCol, const COL_LIST::iterator& _pCol2, _vec3& _vColPos, COL_STATE _eColState);

	void	AddCollider(CCollider* _pCol);
	void	DeleteCollider(CCollider* _pCol);

private:
	map<const _char*, COL_LIST>	m_mCollisionList;

private:
	bool	CollisionCheck(CSphereCollider* _pSphere1, CSphereCollider* _pSphere2);
	bool	CollisionCheck(CSphereCollider* _pSphere, CRayCollider* _pRayCast, _vec3& _vColPos);
	bool	CollisionCheck(CSphereCollider* _pSphere, CCubeCollider* _pCube);
	bool	CollisionCheck(CCubeCollider* _pCube1, CCubeCollider* _pCube2);
	bool	CollisionCheck(CCubeCollider* _pCube, CRayCollider* _pRayCast, _vec3& _vColPos);

public:
	void Release();
};

END

#endif // CollisionManager_h__