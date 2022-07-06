#ifndef SceneManager_h__
#define SceneManager_h__

#include "TemplateSingleton.h"
#include "TinyHashTable.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CSceneManager : public TSingleton<CSceneManager>
{
private:
	explicit CSceneManager();
	virtual ~CSceneManager();

public:
	friend TSingleton;

	const _char*	GetRunningSceneName()	{ return m_pSceneName; }
	RCP_SCENE		GetRunningScene()		{ return m_rcpRunningScene; }
	int				GetObjCount()			{ return (m_rcpRunningScene)?m_rcpRunningScene->GetObjCount():0; }

public:
	void			Update(float _fDeltaTime);

	RCP_SCENE		StartScene(const _char* _pSceneName, _float _fFadeOutTime = 0.f, _bool _bPrevSceneClear = true);

	template<typename T>
	RCPtr<T>		CreateScene() {
		const char* pSceneName = T::GetRTTIInst()->GetClassName();
		RCP_SCENE& pScene = m_hScene[pSceneName];
		if(!pScene) {
			pScene = T::Create(false);
			pScene->SetTag(pSceneName);
		}
		return pScene;
	}

	template<typename T>
	RCPtr<T>		StartScene(_float _fFadeOutTime = 0.f, _bool _bPrevSceneClear = true) {
		if(m_bSwapScene) return NULL;

		const char* pSceneName = T::GetRTTIInst()->GetClassName();
		RCP_SCENE& pScene = m_hScene[pSceneName];
		if(!pScene) {
			pScene = T::Create(false);
			pScene->SetTag(pSceneName);
		}

		return (T*)&*StartScene(pScene, pSceneName, _fFadeOutTime, _bPrevSceneClear);
	}

private:
	typedef TinyHashTable<const _char*, RCP_SCENE> HASH_SCENE;
	HASH_SCENE	m_hScene;
	RCP_SCENE	m_rcpRunningScene;
	RCP_SCENE*	m_pNextScene;
	const _char* m_pSceneName;
	const _char* m_pNextSceneName;
	bool		m_bPrevSceneClear;
	bool		m_bSwapScene;
	float		m_fFadeOutTime;

private:
	RCP_SCENE		StartScene(RCP_SCENE& _pScene, const _char* _pSceneName, _float _fFadeOutTime = 0.f, _bool _bPrevSceneClear = true);

public:
	void Release();
};

END

#endif // SceneManager_h__
