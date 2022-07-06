#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

#include <list>
#include <map>
using namespace std;

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
protected:
	explicit CLayer();
	virtual ~CLayer();

public:
	DeclRTTI(CLayer, CBase);

	virtual void			Enable();
	virtual void			Disable();
	virtual void			Update();

	template<typename T>
	RCPtr<T>				Add_GameObject(bool _bActive = false) { 
		RCPtr<T>& pGameObject = T::Create(this->GetTag(), _bActive);
		m_mGameObjectList[T::GetRTTIInst()->GetClassName()].push_back(pGameObject);
		return pGameObject;
	}

	void					Remove_GameObject(const RCP_GAMEOBJECT& _pObj);
	RCP_GAMEOBJECT			Find_GameObject(const _char* _className);
	list<RCP_GAMEOBJECT>*	Find_GameObjectList(const _char* _className)			{ return &m_mGameObjectList[_className]; }

	int						GetObjCount() {
		MAP_GAMEOBJLIST::iterator Iter = m_mGameObjectList.begin();
		MAP_GAMEOBJLIST::iterator EndIter = m_mGameObjectList.end();
		int c = 0;
		while(Iter != EndIter)
		{
			c += (int)Iter->second.size();
			Iter++;
		}
		return c;
	}

protected:
	typedef map<const _char*, list<RCP_GAMEOBJECT>>	MAP_GAMEOBJLIST;
	MAP_GAMEOBJLIST	m_mGameObjectList;

public:
	CREATE(CLayer);
};

END

TYPEDEF_RCP(Engine::CLayer, LAYER);

#endif // Layer_h__
