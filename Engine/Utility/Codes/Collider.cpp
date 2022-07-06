#include "Collider.h"
#include "CollisionManager.h"
using namespace Engine;

COL_STATE CCollider::GetColState(CCollider* _pCol, bool _bCol)
{
	LIST_PREVCOL::iterator Iter = m_PrevColList.begin();
	LIST_PREVCOL::iterator EndIter = m_PrevColList.end();

	bool PrevCol = false;
	while(Iter != EndIter)
	{
		if(*Iter == _pCol) {
			PrevCol = true;
			break;
		}
		Iter++;
	}

	if(PrevCol)
	{
		if(_bCol) return CS_COL;
		else {
			m_PrevColList.erase(Iter);
			_pCol->Remove_PrevCol(this);
			return CS_EXIT;
		}
	}
	else
	{
		if(_bCol) {
			_pCol->m_PrevColList.push_back(this);
			m_PrevColList.push_back(_pCol);
			return CS_ENTER;
		}
		else return CS_NULL;
	}
}

void CCollider::Enable()
{
	this->Init();
	CCollisionManager::Inst().AddCollider(this);
}

void CCollider::Disable()
{
	LIST_PREVCOL::iterator Iter = m_PrevColList.begin();
	LIST_PREVCOL::iterator EndIter = m_PrevColList.end();

	CCollisionManager* pColMgr = &CCollisionManager::Inst();
	COL_INFO Col1(m_GameObject, _vec3());
	while(Iter != EndIter)
	{
		COL_INFO Col2((*Iter)->GetGameObject(), _vec3());

		if(m_bTrigger || (*Iter)->IsTrigger())
		{
			Col1.pObj->TriggerExit(Col2);
			Col2.pObj->TriggerExit(Col1);
		}
		else
		{
			Col1.pObj->CollisionExit(Col2);
			Col2.pObj->CollisionExit(Col1);
		}
		(*Iter)->Remove_PrevCol(this);
		Iter++;
	}
	m_PrevColList.clear();

	CCollisionManager::Inst().DeleteCollider(this);
}

HRESULT CCollider::Init()
{
	m_pTransform = m_GameObject->GetComponent<CTransform>();
	if(!m_pTransform) m_GameObject->AddComponent<CTransform>();
	return S_OK;
}

void CCollider::Remove_PrevCol(CCollider* _pCol)
{
	LIST_PREVCOL::iterator Iter = m_PrevColList.begin();
	LIST_PREVCOL::iterator EndIter = m_PrevColList.end();

	bool PrevCol = false;
	while(Iter != EndIter)
	{
		if(*Iter == _pCol) {
			m_PrevColList.erase(Iter);
			return;
		}
		Iter++;
	}
}