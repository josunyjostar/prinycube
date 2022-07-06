#include "RayCast.h"
using namespace Engine;

CRayCast::CRayCast()
:m_pCollisionMgr(NULL)
{
}

CRayCast::~CRayCast()
{
}

COL_INFO CRayCast::Picking(const _ray& _rRay, const _char* _pLayerTag)
{
	COL_INFO ColInfo;

	COL_LIST* pColList = m_pCollisionMgr->GetColList(_pLayerTag);

	COL_LIST::iterator Iter		= pColList->begin();
	COL_LIST::iterator EndIter	= pColList->end();

	while(Iter != EndIter)
	{
		if(*Iter == NULL) {
			Iter = pColList->erase(Iter);
			continue;
		}

		if((*Iter)->IsTrigger()) {
			Iter++;
			continue;
		}

		_vec3 vColPos;
		bool bCol = false;

		switch((*Iter)->GetColType())
		{
		case COL_SPHERE:	bCol = CollisionCheck((CSphereCollider*)(*Iter), _rRay, vColPos); break;
		case COL_CUBE:		bCol = CollisionCheck((CCubeCollider*)(*Iter), _rRay, vColPos); break;
		}

		if(bCol && (!ColInfo.pObj || sqrDistance(ColInfo.vColPos, _rRay.Pos) > sqrDistance(vColPos, _rRay.Pos)))
		{
			ColInfo.pObj = (*Iter)->GetGameObject();
			ColInfo.vColPos = vColPos;
		}

		Iter++;
	}

	return ColInfo;
}

bool CRayCast::CollisionCheck(CSphereCollider* _pSphere, const _ray& _rRay, _vec3& _vColPos)
{
	return SphereInRay(_pSphere->GetPosition() + _pSphere->GetTransform()->GetPosition(), _pSphere->GetRadius(), 
		_rRay.Pos, _rRay.Dir, &_vColPos);
}

bool CRayCast::CollisionCheck(CCubeCollider* _pCube, const _ray& _rRay, _vec3& _vColPos)
{
	return BoxInRay(_pCube->GetTransform()->GetWorldMatrix(), _pCube->GetPosition(), _pCube->GetSize(),
		_rRay.Pos, _rRay.Dir, &_vColPos);
}