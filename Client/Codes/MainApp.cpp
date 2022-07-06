#include "stdafx.h"
#include "MainApp.h"

#include "LoadingScene.h"
#include "TitleScene.h"
#include "StageSelectScene.h"

CMainApp::CMainApp()
:m_pGraphicDev(&Engine::CGraphicDev::Inst())
,m_pResourceMgr(&Engine::CResourceManager::Inst())
,m_pShaderMgr(&Engine::CShaderManager::Inst())
,m_pSceneMgr(&Engine::CSceneManager::Inst())
,m_pRenderMgr(&Engine::CRenderManager::Inst())
,m_pFontMgr(&Engine::CFontManager::Inst())
,m_pCallBackMgr(&Engine::CCallbackManager::Inst())
,m_pCamMgr(&Engine::CCameraManager::Inst())
,m_pLightMgr(&Engine::CLightManager::Inst())
,m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
{
}

CMainApp::~CMainApp()
{
	m_pTimer = NULL;
	Engine::Manager_Release();
}

HRESULT CMainApp::Init()
{
	// Init System
	if(FAILED(m_pGraphicDev->Init(g_hWnd, true, WINCX, WINCY))) return E_FAIL;
	RCP_DEVICE pDev = m_pGraphicDev->GetDev();

	Engine::Manager_Init(pDev, RESOURCE_END, 16);

	m_pFontMgr->CreateFont("±¼¸²", 20, 10, FW_HEAVY);
	m_pCamMgr->CreateCamera<CActionCamera>("ActionCam", false);
	m_pCamMgr->CreateCamera<Engine::CCamera>("BasicCam", true);
	m_pLightMgr->CreateLight<Engine::CLight>("DirectionLight");

	pDev->LightEnable(0, true);
	pDev->SetRenderState(D3DRS_LIGHTING, true);
//	pDev->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
//	pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDev->SetRenderState(D3DRS_ZENABLE, true);
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, true);


	g_GameData.Load();


	// Init Scene
	if(!m_pSceneMgr->CreateScene<CLoadingScene>())		return E_FAIL;
	if(!m_pSceneMgr->CreateScene<CTitleScene>())		return E_FAIL;
	if(!m_pSceneMgr->CreateScene<CStageSelectScene>())	return E_FAIL;

	m_pSceneMgr->StartScene<CLoadingScene>()->StartLoading("CTitleScene");

	return S_OK;
}

void CMainApp::Update()
{
	float dt = m_pTimer->GetFrameTime();

#if defined(_DEBUG) || defined(DEBUG)
	if(Engine::KeyInput->IsDown('P'))
		m_pTimer->SetFrameStop(!m_pTimer->GetFrameStop());

	if(Engine::KeyInput->IsDown('S')) 
	{
		g_GameData.RunningStage = 0;
		g_GameData.DeadCount[0] = 99;
		g_GameData.MinDeadCount[0] = 99;
		g_GameData.MinDeadCount[1] = 31;
		g_GameData.MinDeadCount[2] = 28;
		g_GameData.MinDeadCount[3] = 25;
		g_GameData.MinDeadCount[4] = 23;
		g_GameData.MinDeadCount[5] = 1;
		g_GameData.MinDeadCount[6] = 89;
		g_GameData.MinDeadCount[7] = 77;
		g_GameData.ClearStage = 7;
		g_GameData.Save();
	}
#endif

	m_pSceneMgr->Update(dt);
	m_pCallBackMgr->Update(dt);
	m_pCamMgr->Update(dt);
	m_pRenderMgr->Render();

	static int FPS = 0;
	static DWORD dwFPS = GetTickCount();
	static TCHAR szFPS[64];
	++FPS;
	if(dwFPS + 1000 < GetTickCount())
	{
		dwFPS = GetTickCount();
#if defined(_DEBUG) || defined(DEBUG)
		wsprintf(szFPS, L"PRINNY CUBE [FPS : %d / OBJECT : %d]", FPS, m_pSceneMgr->GetObjCount());
#else
		wsprintf(szFPS, L"PRINNY CUBE [FPS : %d]", FPS);
#endif
		FPS = 0;
	}
	SetWindowText(g_hWnd, szFPS);
}