#ifndef LoadingScene_h__
#define LoadingScene_h__

#include "Include.h"
#include "Player.h"
#include "LoadingPrinny.h"

#include <process.h>

class CLoadingScene : public Engine::CScene
{
private:
	explicit CLoadingScene();
	virtual ~CLoadingScene();

public:
	DeclRTTI(CLoadingScene, Engine::CScene);

public:
	const char*			GetLoadSceneName() const	{ return m_pLoadSceneName; }
	CRITICAL_SECTION*	GetCriticalSection()		{ return &m_Crt; }
	RCP_FONT			GetFont() const				{ return m_pFont; }

public:
	virtual HRESULT		Init();
	virtual void		Enable();
	virtual void		Disable();
	virtual void		Update();

	void				StartLoading(const char* _pLoadSceneName);

private:
	Engine::CResourceManager*	m_pResourceMgr;
	Engine::CShaderManager*		m_pShaderMgr;

	RCP_DEVICE			m_pGraphicDev;
	RCP_PLAYER			m_pPlayer;
	RCP_FONT			m_pFont;
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	const char*			m_pLoadSceneName;
	_bool				m_isFinish;

	RCP_LOADINGPRINNY	m_pLoadingPrinny;
	RCP_TIMER			m_pTimer;
	float				m_fDeltaTime;
	float				m_fDelayFinish;
	bool				m_bLoadingEnd;

	_matrix				m_matView;
	_matrix				m_matProj;

	RCP_FADEINOUTFILTER m_pFadeInOutFilter;

private:
	void MakeDove(void* pData);
private:
	HRESULT				LoadScene(const char* _pLoadSceneName);
	static unsigned int APIENTRY Thread_Main(void* _pArg);

public:
	CREATE(CLoadingScene);
};

TYPEDEF_RCP(CLoadingScene, LOADINGSCENE);

#endif // LoadingScene_h__
