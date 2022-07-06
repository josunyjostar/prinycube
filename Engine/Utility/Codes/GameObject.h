#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"
#include "Base.h"
#include "TinyHashTable.h"
#include "Component.h"

#include <map>
using namespace std;

#define AddCallbackFunc(__TYPE,__FUNC_NAME)	FuncInst<__TYPE>()[#__FUNC_NAME]=&__TYPE::__FUNC_NAME;
#define Invoke(__FUNC_NAME,__TIME)			Engine::CallBackMgr->AddFunc(this,__FUNC_NAME,__TIME,NULL)
#define Repeat(__FUNC_NAME,__TIME)			Engine::CallBackMgr->AddFunc(this,__FUNC_NAME,__TIME,NULL,true)
#define InvokeEx(__FUNC_NAME,__TIME,__DATA)	Engine::CallBackMgr->AddFunc(this,__FUNC_NAME,__TIME,__DATA)
#define RepeatEx(__FUNC_NAME,__TIME,__DATA)	Engine::CallBackMgr->AddFunc(this,__FUNC_NAME,__TIME,__DATA,true)
#define StopInvoke(__FUNC_NAME)				Engine::CallBackMgr->DeleteFunc(this,__FUNC_NAME)
#define StopRepeat(__FUNC_NAME)				Engine::CallBackMgr->DeleteFunc(this,__FUNC_NAME)

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject():m_mComponent() {}
	explicit CGameObject(const CGameObject& rhs):CBase(rhs) {}
	virtual ~CGameObject() { this->ClearComponents(); }

public:
	DeclRTTI(CGameObject, CBase);	

	virtual void Enable()	{this->ComponentsEnable();}
	virtual void Disable()	{this->ComponentsDisable();}

//Component Methods
	template <typename T> 
	RCPtr<T> AddComponent() {		
		RCPtr<CComponent>* ppComponent = &m_mComponent[T::GetRTTIInst()->GetClassName()];
		if(!*ppComponent) *ppComponent = CComponent::Create<T>(this);
		return *ppComponent;
	}

	template <typename T> 
	RCPtr<T> GetComponent(){
		return m_mComponent[T::GetRTTIInst()->GetClassName()];
	}

	template <typename T>
	void DeleteComponent(){
		m_mComponent[T::GetRTTIInst()->GetClassName()] = NULL;
	}

	void ComponentsEnable();
	void ComponentsDisable();
	void ClearComponents();

//Collision Methods
	virtual void CollisionEnter(COL_INFO& _ColInfo) {}
	virtual void Collision(COL_INFO& _ColInfo)		{}
	virtual void CollisionExit(COL_INFO& _ColInfo)	{}
	virtual void TriggerEnter(COL_INFO& _ColInfo)	{}
	virtual void Trigger(COL_INFO& _ColInfo)		{}
	virtual void TriggerExit(COL_INFO& _ColInfo)	{}

//CallBack Methods
	template<typename T>
	map<char*, void(T::*)(void*)>& FuncInst()		{ static map<char*, void(T::*)(void*)> mFunc; return mFunc;}

	template<typename T>
	void FunctionCall(T* _pObj, char* _pFuncName, void* _pData) { (_pObj->*FuncInst<T>()[_pFuncName])(_pData); }

protected:
	typedef map<const _char*, RCPtr<CComponent>>	MAP_COMPONENT;
	MAP_COMPONENT									m_mComponent;
};

END

TYPEDEF_RCP(Engine::CGameObject, GAMEOBJECT);

#endif // GameObject_h__
