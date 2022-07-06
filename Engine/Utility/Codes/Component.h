#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent():m_GameObject(NULL) {}
	explicit CComponent(const CComponent& rhs):CBase(rhs) {}
	virtual ~CComponent() {}

public:
	CComponent& operator=(const CComponent& rhs)	{ CBase::operator=(rhs); return *this; }

public:
	void SetGameObject(CGameObject* _pGameObject)	{ m_GameObject = _pGameObject; }
	CGameObject* GetGameObject()					{ return m_GameObject; }

public:
	DeclRTTI(CComponent, CBase);

protected:
	CGameObject* m_GameObject;

public:
	template<typename T>
	static RCPtr<T> Create(CGameObject* _pObj = NULL, bool _bActive = false) {
		T* pInstance = new T;
		pInstance->SetGameObject(_pObj);
		if(FAILED(pInstance->Init()))
		{
			char errMessage[256] = {};
			strcpy_s(errMessage, 256, T::GetRTTIInst()->GetClassName());
			strcat_s(errMessage, " Created Failed");
			MessageBoxA(NULL, errMessage, "Error", MB_OK);
			delete pInstance;
			return NULL;
		}
		if(_bActive) pInstance->SetActive(true);
		return pInstance;
	}
};

END

TYPEDEF_RCP(Engine::CComponent, COMPONENT);

#endif // Component_h__
