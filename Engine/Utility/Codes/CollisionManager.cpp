#include "CollisionManager.h"
using namespace Engine;


CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	this->Release();
}

void CCollisionManager::LayerCollisionCheck(const _char* _pLayerTag1, const _char* _pLayerTag2)
{
	COL_LIST* pLayerColList1 = &m_mCollisionList[_pLayerTag1];
	COL_LIST* pLayerColList2 = &m_mCollisionList[_pLayerTag2];

	COL_LIST::iterator Iter_1		= pLayerColList1->begin();
	COL_LIST::iterator EndIter_1	= pLayerColList1->end();
	COL_LIST::iterator Iter_2		= pLayerColList2->begin();
	COL_LIST::iterator EndIter_2	= pLayerColList2->end();

	while(Iter_1 != EndIter_1)
	{
		if(*Iter_1 == NULL) {
			Iter_1 = pLayerColList1->erase(Iter_1);
			continue;
		}

		while(Iter_2 != EndIter_2) {
			if(*Iter_2 == NULL) {
				Iter_2 = pLayerColList2->erase(Iter_2);
				continue;
			}

			if((*Iter_1)->IsTrigger() && (*Iter_2)->IsTrigger()) continue;

			_vec3 vColPos;

			bool bCol = this->ColliderCollisionCheck(*Iter_1, *Iter_2, vColPos);
			COL_STATE eColState = (*Iter_1)->GetColState(*Iter_2, bCol);

			if((*Iter_1)->IsTrigger() || (*Iter_2)->IsTrigger())
				this->CallTrigger(Iter_1, Iter_2, vColPos, eColState);
			else
				this->CallCollision(Iter_1, Iter_2, vColPos, eColState);

			if(*Iter_1 == NULL) break;
			Iter_2++;
		}

		Iter_1++;
	}
}

bool CCollisionManager::ColliderCollisionCheck(CCollider* _pCol, CCollider* _pCol2, _vec3& _pColPos)
{
	switch(_pCol->GetColType())
	{
	case COL_SPHERE:
		switch(_pCol2->GetColType())
		{
		case COL_SPHERE:	return CollisionCheck((CSphereCollider*)_pCol, (CSphereCollider*)_pCol2);
		case COL_CUBE:		return CollisionCheck((CSphereCollider*)_pCol, (CCubeCollider*)_pCol2);
		case COL_RAY:		return CollisionCheck((CSphereCollider*)_pCol, (CRayCollider*)_pCol2, _pColPos);
		} break;
	case COL_CUBE:
		switch(_pCol2->GetColType())
		{
		case COL_SPHERE:	return CollisionCheck((CSphereCollider*)_pCol2, (CCubeCollider*)_pCol);
		case COL_CUBE:		return CollisionCheck((CCubeCollider*)_pCol, (CCubeCollider*)_pCol2);
		case COL_RAY:		return CollisionCheck((CCubeCollider*)_pCol, (CRayCollider*)_pCol2, _pColPos);
		}
		break;
	case COL_RAY:
		switch(_pCol2->GetColType())
		{
		case COL_SPHERE:	return CollisionCheck((CSphereCollider*)_pCol2, (CRayCollider*)_pCol, _pColPos);
		case COL_CUBE:		return CollisionCheck((CCubeCollider*)_pCol2, (CRayCollider*)_pCol, _pColPos);
		}
		break;
	}

	return false;
}

void CCollisionManager::CallCollision(const COL_LIST::iterator& _pCol, const COL_LIST::iterator& _pCol2, _vec3& _pColPos, COL_STATE _eColState)
{
	if(_eColState == CS_NULL) return;

	COL_INFO Col1((*_pCol)->GetGameObject(), _pColPos), Col2((*_pCol2)->GetGameObject(), _pColPos);

	switch(_eColState)
	{
	case CS_ENTER:
		Col1.pObj->CollisionEnter(Col2);
		Col2.pObj->CollisionEnter(Col1);
		if(*_pCol == NULL || *_pCol2 == NULL) break;
	case CS_COL:
		Col1.pObj->Collision(Col2);
		Col2.pObj->Collision(Col1);
		break;
	case CS_EXIT:
		Col1.pObj->CollisionExit(Col2);
		Col2.pObj->CollisionExit(Col1);
		break;
	}
}

void CCollisionManager::CallTrigger(const COL_LIST::iterator& _pCol, const COL_LIST::iterator& _pCol2, _vec3& _pColPos, COL_STATE _eColState)
{
	if(_eColState == CS_NULL) return;

	COL_INFO Col1((*_pCol)->GetGameObject(), _pColPos), Col2((*_pCol2)->GetGameObject(), _pColPos);

	switch(_eColState)
	{
	case CS_ENTER:
		Col1.pObj->TriggerEnter(Col2);
		Col2.pObj->TriggerEnter(Col1);
		if(*_pCol == NULL || *_pCol2 == NULL) break;
	case CS_COL:
		Col1.pObj->Trigger(Col2);
		Col2.pObj->Trigger(Col1);
		break;
	case CS_EXIT:
		Col1.pObj->TriggerExit(Col2);
		Col2.pObj->TriggerExit(Col1);
		break;
	}
}

void CCollisionManager::AddCollider(CCollider* _pCollider)
{
	m_mCollisionList[_pCollider->GetGameObject()->GetTag()].push_back(_pCollider);
}

void CCollisionManager::DeleteCollider(CCollider* _pCollider)
{
	COL_LIST* pLayerColList = &m_mCollisionList[_pCollider->GetGameObject()->GetTag()];
	COL_LIST::iterator Iter = pLayerColList->begin();
	COL_LIST::iterator EndIter = pLayerColList->end();

	while(Iter != EndIter)
	{
		if((*Iter) == _pCollider)
		{
			(*Iter) = NULL;
			return;
		}
		Iter++;
	}
}

bool CCollisionManager::CollisionCheck(CSphereCollider* _pSphere1, CSphereCollider* _pSphere2)
{
	return SphereInSphere(_pSphere1->m_vPosition + _pSphere1->m_pTransform->GetPosition(), _pSphere1->m_fRadius, 
		_pSphere2->m_vPosition + _pSphere2->m_pTransform->GetPosition(), _pSphere2->m_fRadius);
}

bool CCollisionManager::CollisionCheck(CSphereCollider* _pSphere, CRayCollider* _pRayCast, _vec3& _vColPos)
{
	return SphereInRay(_pSphere->m_vPosition + _pSphere->m_pTransform->GetPosition(), _pSphere->m_fRadius, 
		_pRayCast->m_vPosition + _pRayCast->m_pTransform->GetPosition(), TransformNormal(_pRayCast->m_vDirection, _pRayCast->m_pTransform->GetWorldMatrix()), &_vColPos);
}

bool CCollisionManager::CollisionCheck(CSphereCollider* _pSphere, CCubeCollider* _pCube)
{
	return SphereInBox(_pSphere->m_vPosition + _pSphere->m_pTransform->GetPosition(), _pSphere->m_fRadius, 
		_pCube->m_pTransform->GetWorldMatrix(), _pCube->m_vPosition, _pCube->m_vSize);
}

bool CCollisionManager::CollisionCheck(CCubeCollider* _pCube1, CCubeCollider* _pCube2)
{
	return BoxInBox(_pCube1->m_pTransform->GetWorldMatrix(), _pCube1->m_vPosition, _pCube1->m_vSize,
		_pCube2->m_pTransform->GetWorldMatrix(), _pCube2->m_vPosition, _pCube2->m_vSize);
}

bool CCollisionManager::CollisionCheck(CCubeCollider* _pCube, CRayCollider* _pRayCast, _vec3& _vColPos)
{
	return BoxInRay(_pCube->m_pTransform->GetWorldMatrix(), _pCube->m_vPosition, _pCube->m_vSize,
		_pRayCast->m_vPosition + _pRayCast->m_pTransform->GetPosition(), TransformNormal(_pRayCast->m_vDirection, _pRayCast->m_pTransform->GetWorldMatrix()), &_vColPos);
}

void CCollisionManager::Release()
{
	m_mCollisionList.clear();
}