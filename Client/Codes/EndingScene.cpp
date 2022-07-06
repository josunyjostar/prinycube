#include "stdafx.h"
#include "EndingScene.h"

#include "BTSceneChange.h"
#include "BackGround.h"
#include "BasicCube.h"
#include "UI.h"
#include "Dove.h"
#include "Fish.h"
#include "JellyFish.h"
#include "TitleScene.h"
#include "RandRotateObj.h"
#include "ItemHeart.h"

CEndingScene::CEndingScene()
:m_pDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_fDeltaTime(0.f)
,m_fPressKey(0.f)
,m_iPrevLife(3)
{
}

CEndingScene::~CEndingScene()
{
}

void CEndingScene::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_ENDING))
	{
		Engine::SoundMgr->AllStop();		
		Engine::SoundMgr->Play(BGM_ENDING);
	}

	_matrix matView, matProj;

	D3DXMatrixLookAtLH(&matView, &_vec3(-2.f, 13.f, -20.f), &_vec3(-2.f, 3.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixOrthoLH(&matProj, 10.f*WINCX/WINCY, 10.f, -0.1f, 1000.f);

	RCP_CAMERA pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(matView);
	pMainCam->SetProjMatrix(matProj);

	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_FADEINOUTFILTER pFadeInOutFilter = pUILayer->Add_GameObject<CFadeOutFilter>(true);
	pFadeInOutFilter->FadeOut();

	pUILayer->Add_GameObject<CBackGround>(true)->SetTexture(RESOURCE_STATIC, "TEX_SEA_BGA1");

	RCP_OBJECTFONT pObjFont = pUILayer->Add_GameObject<CObjectFont>(true);
	pObjFont->GetComponent<Engine::CRenderer>()->SetZOrder(1);
	m_pFont = pObjFont->GetFont();
	m_pFont->SetColor(D3DCOLOR_XRGB(255, 255, 255));
	m_pFont->SetPosition(_vec2(64.f, WINCY-64.f));

	RCP_UI pUI = pUILayer->Add_GameObject<CUI>(true);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_TITLE_NAME");
	pUI->SetScale(_vec2(512.f, 196.f));
	pUI->SetPosition(_vec2(WINCX*2.9f/4.f, WINCY/4.f));

	pUI = pUILayer->Add_GameObject<CUI>(true);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_F_CREATORS");
	pUI->SetScale(_vec2(512.f, 512.f));
	pUI->SetPosition(_vec2(WINCX*1.f/4.f, WINCY/2.f));

	m_pPrinny = this->Create_Layer("Prinny")->Add_GameObject<CEndingPrinny>(true);

	RCP_TRANSFORM pTransform = this->Create_Layer("Tile")->Add_GameObject<CBasicCube>(true)->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(5.f, 0.5f, 30.f));
	pTransform->SetPosition(_vec3(0,-1,0));

	for(int i = 0; i < 3; ++i)
	{
		m_pLife[i] = pUILayer->Add_GameObject<CLife>(true);
		m_pLife[i]->GetComponent<Engine::CTransform>()->SetPosition(UI_POS(WINCX-64.f-64*i, WINCY-64.f));
	}

	Engine::CLightManager::Inst().GetLight("DirectionLight")->DeviceInLight();
}

void CEndingScene::Update()
{
	CScene::Update();

	m_fDeltaTime = m_pTimer->GetFrameTime();

	m_fPressKey += m_fDeltaTime;

	if(m_fPressKey < 1.5f)
		m_pFont->SetString("(PRESS ENTER KEY)");
	else if(m_fPressKey < 3.f)
		m_pFont->SetString("");
	else m_fPressKey = 0.f;

	int iLife = m_pPrinny->GetLife();
	if(m_iPrevLife != iLife)
	{
		for(int i = 0; i < 3; ++i) {
			if(i >= iLife && m_pLife[i]->GetActive()) m_pLife[i]->SetActive(false);
			else if(i < iLife && !m_pLife[i]->GetActive()) m_pLife[i]->SetActive(true);
		}
		m_iPrevLife = iLife;
	}

	static float fObjCreateTime = 1.5f;
	fObjCreateTime -= m_fDeltaTime;

	if(fObjCreateTime < 0.f) {
		fObjCreateTime = RAND_RANGE_F(0.5f, 1.f);

		this->Create_Layer("ObjectLayer")->Add_GameObject<CRandRotateObj>(true);

		if(!(rand()%20)) this->Create_Layer("ObjectLayer")->Add_GameObject<CItemHeart>(true);
	}

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Prinny", "ObjectLayer");
}