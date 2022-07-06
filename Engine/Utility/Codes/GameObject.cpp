#include "GameObject.h"
using namespace Engine;

void CGameObject::ComponentsEnable()
{
	MAP_COMPONENT::iterator Iter = m_mComponent.begin();
	MAP_COMPONENT::iterator EndIter = m_mComponent.end();
	while(Iter != EndIter)
	{
		if(Iter->second) Iter->second->SetActive(true);
		Iter++;
	}
}

void CGameObject::ComponentsDisable()
{
	MAP_COMPONENT::iterator Iter = m_mComponent.begin();
	MAP_COMPONENT::iterator EndIter = m_mComponent.end();
	while(Iter != EndIter)
	{
		if(Iter->second) Iter->second->SetActive(false);
		Iter++;
	}
}

void CGameObject::ClearComponents()
{
	MAP_COMPONENT::iterator Iter = m_mComponent.begin();
	MAP_COMPONENT::iterator EndIter = m_mComponent.end();
	while(Iter != EndIter)
	{
		if(Iter->second) Iter->second->SetActive(false);
		Iter++;
	}
	m_mComponent.clear();
}