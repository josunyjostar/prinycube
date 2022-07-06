#include "stdafx.h"
#include "Stage06_2.h"
#include "Stage07_1.h"

#include "Player.h"
#include "BasicCube.h"
#include "SkyBox.h"
#include "Wall.h"
#include "MoveCube.h"
#include "Ocean.h"
#include "BridgeCube.h"
#include "StageSwapCube.h"
#include "BackGround.h"
#include "TempleCube.h"
#include "Light.h"
#include "Dust.h"
#include "ToolTip.h"
#include "Fountain.h"
#include "Dead.h"
#include "DeadWater.h"
#include "ArrowTrapCube.h"
#include "NextCube.h"
#include "PickUpCube.h"
#include "BTMenu.h"

CStage06_2::CStage06_2()
{
}

CStage06_2::~CStage06_2()
{
}

void CStage06_2::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 6;

	Engine::CSceneManager::Inst().CreateScene<CStage07_1>();

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
	pPlayer->SetSpawnPos(VEC3EX(0,7,0));
	pPlayer->GetTransform()->SetPosition(VEC3EX(0,7,0));

	RCP_TRANSFORM pTransform;
	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_BACKGROUND& pBackGround = pUILayer->Add_GameObject<CBackGround>(true);
	pUILayer->Add_GameObject<CBTMenu>(true);
	m_pFadeInOutFilter = pUILayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->SetPosition(pPlayer->GetTransform()->GetScreenPos(m_pDev, WINCX, WINCY));
	m_pFadeInOutFilter->FadeOut(3.f, FADE_OUT);
	m_pFadeInOutFilter->SetColor(_vec4(1,1,1,1));
	m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));

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
	for (int i = 0 ; i < 5; i++)
	{
		pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(2-i,5,0), m_pShadowLight);
	}


	for (int i = 0 ; i < 5; i++)
	{
		pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(0,5,2-i), m_pShadowLight);
	}

	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-1,4,2), DIR_RB, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-1,3,2), DIR_RB, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-1,2,2), DIR_RB, 0.25f, m_pShadowLight);

	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(2,4,1), DIR_LB, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(2,3,1), DIR_LB, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(2,2,1), DIR_LB, 0.25f, m_pShadowLight);

	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(1,4,-2), DIR_LT, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(1,3,-2), DIR_LT, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(1,2,-2), DIR_LT, 0.25f, m_pShadowLight);

	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-2,4,-1), DIR_RT, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-2,3,-1), DIR_RT, 0.25f, m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-2,2,-1), DIR_RT, 0.25f, m_pShadowLight);

	pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(4.5f,0,2), m_pShadowLight);
	pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(2,0,-4.5f), m_pShadowLight);
	pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(-4.5f,0,-2), m_pShadowLight);
	pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(-2,0,4.5f), m_pShadowLight);

	RCP_BASICCUBE pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(5.5f,0,2), m_pShadowLight);

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(2,0,-5.5f), m_pShadowLight);

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5.5f,0,-2), m_pShadowLight);

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-2,0,5.5f), m_pShadowLight);

	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this,VEC3EX(5.5f,1,2), pPlayer, "CStage07_1", m_pFadeInOutFilter, m_pShadowLight);
	pBasicCube = pLayer->Add_GameObject<CTempleLightSource>(true);
	pBasicCube->SetShadow(this, VEC3EX(5.5f,1,2), m_pShadowLight);

	pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(2,1,-5.5f), pPlayer, "CStage07_1", m_pFadeInOutFilter, m_pShadowLight);
	pTempleLight->SetFake(true);
	pBasicCube = pLayer->Add_GameObject<CTempleLightSource>(true);
	pBasicCube->SetShadow(this, VEC3EX(2,1,-5.5f), m_pShadowLight);

	pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(-5.5f,1,-2), pPlayer, "CStage07_1", m_pFadeInOutFilter, m_pShadowLight);
	pTempleLight->SetFake(true);
	pBasicCube = pLayer->Add_GameObject<CTempleLightSource>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5.5f,1,-2), m_pShadowLight);

	pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(-2,1,5.5f), pPlayer, "CStage07_1", m_pFadeInOutFilter, m_pShadowLight);
	pTempleLight->SetFake(true);
	pBasicCube = pLayer->Add_GameObject<CTempleLightSource>(true);
	pBasicCube->SetShadow(this, VEC3EX(-2,1,5.5f), m_pShadowLight);

	RCP_OCEAN pOcean;
	pOcean = this->Create_Layer("Tile")->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));

	InitBubble();
}

void CStage06_2::Disable()
{
	this->Clear();
}

void CStage06_2::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Wall", "Arrow");

	CStageScene::ClickEvent();

	RegenBubble();
}
