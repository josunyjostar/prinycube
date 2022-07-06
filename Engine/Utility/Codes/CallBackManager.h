#ifndef CallBackManager_h__
#define CallBackManager_h__

#include "Engine_Include.h"
#include "TemplateSingleton.h"
#include "GameObject.h"

BEGIN(Engine)

typedef struct tagTimer {
	char* pFuncName;
	void* pData;
	float fMaxTime;
	float fDelayTime;
	bool bRepeat;

	virtual void Call() = 0;
	virtual ~tagTimer(){}
}tTimer;

template<typename T>
struct BindingTimer : public tTimer{
	T* pObj;
	virtual void Call() {if(pObj->GetActive()) pObj->FunctionCall(pObj, pFuncName, pData);}
};

#define MAX_TIMER	512

class ENGINE_DLL CCallbackManager : public TSingleton<CCallbackManager>
{
private:
	explicit CCallbackManager(void);
	virtual ~CCallbackManager(void);

public:
	friend TSingleton;

	void Update(const float& _fDeltaTime);

	template<typename T>
	void AddFunc(T* _pObj, char* _pFuncName, float _fTime, void* _pData, bool _bRepeat = false)
	{
		BindingTimer<T>* pTimer = new BindingTimer<T>;
		pTimer->pObj = _pObj;
		pTimer->pFuncName = _pFuncName;
		pTimer->fMaxTime = _fTime;
		pTimer->fDelayTime = _fTime;
		pTimer->pData = _pData;
		pTimer->bRepeat = _bRepeat;

		for(int i = 0; i < m_iSize; ++i) {
			if(m_Timer[i]) continue;
			m_Timer[i] = pTimer;
			return;
		}
		m_Timer[m_iSize++] = pTimer;
	}

	void DeleteFunc(CGameObject* _pObj, char* _pFuncName);

private:
	tTimer*	m_Timer[MAX_TIMER];
	int		m_iSize;

public:
	void Release();
};

extern CCallbackManager* const CallBackMgr;

END

#endif // CallBackManager_h__