#include "stdafx.h"
#include "Stage04.h"
#include "Stage05.h"

#include "Player.h"
#include "SkyBox.h"
#include "Wall.h"

#include "BasicCube.h"
#include "MoveCube.h"
#include "BridgeCube.h"
#include "TempleCube.h"
#include "NextCube.h"
#include "StageSwapCube.h"
#include "PickUpCube.h"

#include "Ocean.h"

#include "BackGround.h"

#include "Light.h"
#include "Dust.h"

#include "ToolTip.h"

#include "Fountain.h"
#include "Dead.h"
#include "DeadWater.h"
#include "BTMenu.h"
#include "StagePopup.h"

CStage04::CStage04()
{
}

CStage04::~CStage04()
{
}

void CStage04::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 4;
	Engine::CSceneManager::Inst().CreateScene<CStage05>();
	
	m_pShadowLight = this->Create_Layer("ShadowLight")->Add_GameObject<CShadowLight>(true);

	RCP_LIGHT pLight = Engine::CLightManager::Inst().GetLight("DirectionLight");
	pLight->DeviceInLight();

	m_pDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	RCP_CAMERA& pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(m_matView);
	pMainCam->SetProjMatrix(m_matProj);

	RCP_LAYER& pLayer = this->Create_Layer("Player");
	RCP_PLAYER& pPlayer = pLayer->Add_GameObject<CPlayer>(true);
	pPlayer->SetShadow(this, m_pShadowLight);
	pPlayer->SetSpawnPos(_vec3(0,-1,0));

	RCP_TRANSFORM pTransform;
	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_BACKGROUND& pBackGround = pUILayer->Add_GameObject<CBackGround>(true);
	pUILayer->Add_GameObject<CBTMenu>(true);
	m_pFadeInOutFilter = pUILayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->SetPosition(pPlayer->GetTransform()->GetScreenPos(m_pDev, WINCX, WINCY));
	m_pFadeInOutFilter->FadeOut(3.f, FADE_OUT);
	m_pFadeInOutFilter->SetColor(_vec4(1,1,1,1));
	m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));
	RCP_STAGEPOPUP pStagePopup = pUILayer->Add_GameObject<CStagePopup>();

	pLayer = this->Create_Layer("Wall");

	RCP_WALL pWall;
	pWall = pLayer->Add_GameObject<CWall>(true);
	pWall->SetRenderTarget(m_pShadowRenderTarget[WALL_LEFT]);
	pWall->SetDepthBuffer(m_pShadowDepthStencil[WALL_LEFT]);
	pWall->SetLightProj(m_matLightProj);
	pWall->SetShadowLight(m_pShadowLight);
	pWall->SetTexture(RESOURCE_STATIC, "TEX_WALL", 0);
	pTransform = pWall->GetComponent<Engine::CTransform>();
	pTransform->SetPosition(VEC_LT(7.5)+VEC_Y(2.5));
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI*0.25f);
	pWall->SetDiffuse(Dot(Normalize(_vec3(1.f, 0.f, -1.f)), -(_vec3)pLight->GetLight().Direction));

	pWall = pLayer->Add_GameObject<CWall>(true);
	pWall->SetRenderTarget(m_pShadowRenderTarget[WALL_RIGHT]);
	pWall->SetDepthBuffer(m_pShadowDepthStencil[WALL_RIGHT]);
	pWall->SetLightProj(m_matLightProj);
	pWall->SetShadowLight(m_pShadowLight);
	pWall->SetTexture(RESOURCE_STATIC, "TEX_WALL", 1);
	pTransform = pWall->GetComponent<Engine::CTransform>();
	pTransform->SetPosition(VEC_RT(7.5)+VEC_Y(2.5));
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pWall->SetDiffuse(Dot(Normalize(_vec3(-1.f, 0.f, -1.f)), -(_vec3)pLight->GetLight().Direction));

	pWall = pLayer->Add_GameObject<CWall>(true);
	pWall->SetTexture(RESOURCE_STATIC, "TEX_TILE", 0);
	pTransform = pWall->GetComponent<Engine::CTransform>();
	pTransform->SetPosition(_vec3(0.f, -5.f, 0.f));
	pTransform->Rotate(_vec3(1.f, 0.f, 0.f), PI*0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pWall->SetDiffuse(Dot(_vec3(0.f, 1.f, 0.f), -(_vec3)pLight->GetLight().Direction));

	pLayer = this->Create_Layer("Tile");

	RCP_STAGESWAPCUBE pStageSwapCube1 = pLayer->Add_GameObject<CStageSwapCube>(true);
	pStageSwapCube1->SetShadow(this, VEC3EX(-5,-2,-2), m_pShadowLight);
	pStageSwapCube1->SetPlayer(pPlayer);
	pStageSwapCube1->SetHide(false,false);
	pStageSwapCube1->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SKY_BGA4", RESOURCE_STATIC, "TEX_SKY_BGA3");

	RCP_PICKUPCUBE pPickCube = this->Create_Layer("PickCube")->Add_GameObject<CPickUpCube>(true);
	pPickCube->SetShadow(this, VEC3EX(-5,-1,-3) , m_pShadowLight);
	pPickCube->SetHide(true,true);
	pStageSwapCube1->AddCube(pPickCube);

	RCP_TOOLTIPTRIGGER pToolTip = pLayer->Add_GameObject<CToolTipTrigger>(true);
	pToolTip->SetPosition(VEC3EX(-5,-1,-3));
	pToolTip->SetToolTip(RESOURCE_STATIC, "TEX_F_TOOLTIP_PICKUP", 4.f, 0.5f, 0.5f, _vec2(320.f, 80.f), _vec2(120.f, -60.f));

	//// START CUBE POS
	RCP_BASICCUBE pBasicCube;
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(0,-2,0), m_pShadowLight);

	for(int i = 0 ; i < 5 ;++i)
	{		
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(-4+i,-2,-2), m_pShadowLight);
	}

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(1,-0.5,-2), m_pShadowLight);
	pBasicCube->GetComponent<Engine::CTransform>()->SetScaleY(2.5f);

	for(int i = 0 ; i < 6 ;++i)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(1,1,-1+i), m_pShadowLight);

	}

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(1,1,5), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(1,2.5f,5), pPlayer, "CStage05", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC3EX(1,2.5f,5), m_pShadowLight);


	//---------------------------------------------------------------------------------------------------
	RCP_OCEAN pOcean;
	pOcean = this->Create_Layer("Tile")->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));
	pBasicCube = pLayer->Add_GameObject<CTempleLightSource>(true);
}

void CStage04::Disable()
{
	this->Clear();
}

void CStage04::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Light");

	CStageScene::ClickEvent();
}
