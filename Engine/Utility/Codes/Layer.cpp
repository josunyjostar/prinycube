#include "Layer.h"
using namespace Engine;

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

void CLayer::Enable()
{
	MAP_GAMEOBJLIST::iterator mIter		= m_mGameObjectList.begin();
	MAP_GAMEOBJLIST::iterator mEndIter	= m_mGameObjectList.end();

	while(mIter != mEndIter)
	{
		list<RCP_GAMEOBJECT>::iterator Iter = mIter->second.begin();
		list<RCP_GAMEOBJECT>::iterator EndIter = mIter->second.end();

		while(Iter != EndIter) {
			if(*Iter) 
			{
				(*Iter)->SetActive(true);
				Iter++;
			}
			else Iter = mIter->second.erase(Iter);	
		}
		mIter++;
	}
}

void CLayer::Disable()
{
	MAP_GAMEOBJLIST::iterator mIter		= m_mGameObjectList.begin();
	MAP_GAMEOBJLIST::iterator mEndIter	= m_mGameObjectList.end();

	while(mIter != mEndIter)
	{
		list<RCP_GAMEOBJECT>::iterator Iter = mIter->second.begin();
		list<RCP_GAMEOBJECT>::iterator EndIter = mIter->second.end();

		while(Iter != EndIter) {
			if(*Iter) 
			{
				(*Iter)->SetActive(false);
				Iter++;
			}
			else Iter = mIter->second.erase(Iter);	
		}
		mIter++;
	}
}

void CLayer::Update()
{
	MAP_GAMEOBJLIST::iterator mIter		= m_mGameObjectList.begin();
	MAP_GAMEOBJLIST::iterator mEndIter	= m_mGameObjectList.end();

	while(mIter != mEndIter)
	{
		list<RCP_GAMEOBJECT>::iterator Iter = mIter->second.begin();
		list<RCP_GAMEOBJECT>::iterator EndIter = mIter->second.end();

		while(Iter != EndIter) {
			if(*Iter) 
			{
				(*Iter)->Update();
				Iter++;
			}
			else Iter = mIter->second.erase(Iter);	
		}
		mIter++;
	}
}

void CLayer::Remove_GameObject(const RCP_GAMEOBJECT& _pObj)
{
	const _char* pClassName = _pObj->GetRTTI()->GetClassName();
	MAP_GAMEOBJLIST::iterator Iter = m_mGameObjectList.find(pClassName);

	if(Iter == m_mGameObjectList.end()) return;

	list<RCP_GAMEOBJECT>* pList = &Iter->second;
	list<RCP_GAMEOBJECT>::iterator ListIter = pList->begin();
	list<RCP_GAMEOBJECT>::iterator ListEndIter = pList->end();

	while(ListIter != ListEndIter) {
		if((*ListIter) == _pObj)
		{
			(*ListIter)->SetActive(false);
			*ListIter = NULL;
			return;
		}
		ListIter++;
	}

	return;
}

RCP_GAMEOBJECT CLayer::Find_GameObject(const _char* _className)
{
	list<RCP_GAMEOBJECT>* pList = &m_mGameObjectList[_className];
	list<RCP_GAMEOBJECT>::iterator Iter = pList->begin();
	list<RCP_GAMEOBJECT>::iterator EndIter = pList->end();

	while(Iter != EndIter) {
		if((*Iter)->GetRTTI()->GetClassName() == _className)
			return *Iter;
		Iter++;
	}

	return NULL;
}