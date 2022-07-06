#include "stdafx.h"
#include "Stage07_1.h"
#include "Stage07_2.h"

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
#include "ArrowTrapCube.h"

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

CStage07_1::CStage07_1()
{
}

CStage07_1::~CStage07_1()
{
}

void CStage07_1::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 7;
	Engine::CSceneManager::Inst().CreateScene<CStage07_2>();

	m_pShadowLight = this->Create_Layer("ShadowLight")->Add_GameObject<CShadowLight>(true);

	RCP_LIGHT pLight = Engine::CLightManager::Inst().GetLight("DirectionLight");
	pLight->DeviceInLight();

	RCP_CAMERA& pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(m_matView);
	pMainCam->SetProjMatrix(m_matProj);

	RCP_LAYER& pLayer = this->Create_Layer("Player");
	RCP_PLAYER& pPlayer = pLayer->Add_GameObject<CPlayer>(true);
	pPlayer->SetShadow(this, m_pShadowLight);

	RCP_BACKGROUND& pBackGround = pLayer->Add_GameObject<CBackGround>(true);
	RCP_TRANSFORM pTransform;

	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	m_pFadeInOutFilter = pUILayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->FadeOut(3.f, FADE_OUT); 
	m_pFadeInOutFilter->SetColor(_vec4(1,1,1,1));
	m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));
	RCP_STAGEPOPUP pStagePopup = pUILayer->Add_GameObject<CStagePopup>();

	pUILayer->Add_GameObject<CBTMenu>(true);

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
	pStageSwapCube1->SetShadow(this, _vec3(), m_pShadowLight);
	pStageSwapCube1->SetPlayer(pPlayer);
	pStageSwapCube1->SetHide(false,false);
	pStageSwapCube1->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA4", RESOURCE_STATIC, "TEX_SEA_BGA5");

	RCP_STAGESWAPCUBE pStageSwapCube2 = pLayer->Add_GameObject<CStageSwapCube>(true);
	pStageSwapCube2->SetShadow(this, _vec3(0,3,3), m_pShadowLight);
	pStageSwapCube2->SetPlayer(pPlayer);
	pStageSwapCube2->SetHide(false,false);
	pStageSwapCube2->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA4", RESOURCE_STATIC, "TEX_SEA_BGA5");

	RCP_PICKUPCUBE pPickCube1 = this->Create_Layer("PickCube")->Add_GameObject<CPickUpCube>(true);
	pPickCube1->SetShadow(this, VEC3EX(-5,0,-4) , m_pShadowLight);
	pPickCube1->SetHide(true,true);
	pStageSwapCube1->AddCube(pPickCube1);

	RCP_PICKUPCUBE pPickCube2 = this->Create_Layer("PickCube")->Add_GameObject<CPickUpCube>(true);
	pPickCube2->SetShadow(this, VEC3EX(4,0,5) , m_pShadowLight);
	pPickCube2->SetHide(true,true);
	pStageSwapCube1->AddCube(pPickCube2);

	RCP_BASICCUBE pBasicCube;

	for(int i=0; i < 10; ++i)
	{	
		for(int j=0; j < 10; ++j)
		{	
			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(-5+j,-1,-4+i), m_pShadowLight);
			pStageSwapCube2->AddCube(pBasicCube);
		}
	}

	//Å¬¸®¾î
	RCP_NEXTCUBE pNextCube = pLayer->Add_GameObject<CNextCube>(true);
	pNextCube->Setting(this,VEC3EX(1,-4,-2), pPlayer, "CStage07_2", m_pFadeInOutFilter, m_pShadowLight);

	pLayer = this->Create_Layer("Arrow");
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(1, 1, 3), DIR_LB, RAND_RANGE_F(1.5f, 2.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(0, 1, 3), DIR_LB, RAND_RANGE_F(2.5f, 3.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(1, -1, -6), DIR_RT, RAND_RANGE_F(1.5f, 2.25f), m_pShadowLight);

	RCP_OCEAN pOcean;
	pOcean = this->Create_Layer("Tile")->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));

	InitSeaFood();
}

void CStage07_1::Disable()
{
	this->Clear();
}

void CStage07_1::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Light");

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Wall", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Tile", "Arrow");

	Engine::CCollisionManager::Inst().LayerCollisionCheck("PickCube", "Arrow");

	CStageScene::ClickEvent();

	RegenSeaFood();
}
