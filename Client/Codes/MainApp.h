#ifndef MAINAPP_H__
#define MAINAPP_H__

#include "Include.h"

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	DeclRTTI(CMainApp, CBase);

	HRESULT Init();
	void	Update();

private:
	Engine::CGraphicDev*		m_pGraphicDev;
	Engine::CResourceManager*	m_pResourceMgr;
	Engine::CShaderManager*		m_pShaderMgr;
	Engine::CSceneManager*		m_pSceneMgr;
	Engine::CRenderManager*		m_pRenderMgr;
	Engine::CFontManager*		m_pFontMgr;
	Engine::CCallbackManager*	m_pCallBackMgr;
	Engine::CCameraManager*		m_pCamMgr;
	Engine::CLightManager*		m_pLightMgr;
	RCP_TIMER					m_pTimer;

public:
	CREATE(CMainApp);
};

TYPEDEF_RCP(CMainApp, MAINAPP);

#endif //MAINAPP_H__