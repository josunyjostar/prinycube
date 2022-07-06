#include "stdafx.h"
#include "Stage06_1.h"
#include "Stage06_2.h"

#include "Player.h"
#include "SkyBox.h"
#include "Wall.h"
#include "BasicCube.h"
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
#include "StagePopup.h"

CStage06_1::CStage06_1()
{
}

CStage06_1::~CStage06_1()
{
	if(m_pStageSwapCube1) m_pStageSwapCube1->Clear();
	if(m_pStageSwapCube2) m_pStageSwapCube2->Clear();
	if(m_pStageSwapCube3) m_pStageSwapCube3->Clear();
	if(m_pStageSwapCube4) m_pStageSwapCube4->Clear();
}

void CStage06_1::Enable()
{
	CScene::Enable();

	Engine::SoundMgr->AllStop();
	Engine::SoundMgr->Play(BGM_STAGE_06);

	g_GameData.RunningStage = 6;
	Engine::CSceneManager::Inst().CreateScene<CStage06_2>();

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

	m_pStageSwapCube1 = pLayer->Add_GameObject<CStageSwapCube>(true);
	m_pStageSwapCube1->SetShadow(this, VEC3EX(2,2,0), m_pShadowLight);
	m_pStageSwapCube1->SetPlayer(pPlayer);
	m_pStageSwapCube1->SetHide(false,true);
	m_pStageSwapCube1->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA1", RESOURCE_STATIC, "TEX_SEA_BGA3");

	m_pStageSwapCube2 = pLayer->Add_GameObject<CStageSwapCube>(true);
	m_pStageSwapCube2->SetShadow(this, VEC3EX(0,4,2), m_pShadowLight);
	m_pStageSwapCube2->SetPlayer(pPlayer);
	m_pStageSwapCube2->SetHide(true,true);
	m_pStageSwapCube2->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA1", RESOURCE_STATIC, "TEX_SEA_BGA3");
	m_pStageSwapCube1->AddCube(m_pStageSwapCube2);
	m_pStageSwapCube2->AddCube(m_pStageSwapCube1);

	m_pStageSwapCube3 = pLayer->Add_GameObject<CStageSwapCube>(true);
	m_pStageSwapCube3->SetShadow(this, VEC3EX(4,2,6), m_pShadowLight);
	m_pStageSwapCube3->SetPlayer(pPlayer);
	m_pStageSwapCube3->SetHide(true,true);
	m_pStageSwapCube3->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA1", RESOURCE_STATIC, "TEX_SEA_BGA3");
	m_pStageSwapCube2->AddCube(m_pStageSwapCube3);
	m_pStageSwapCube3->AddCube(m_pStageSwapCube2);

	m_pStageSwapCube4 = pLayer->Add_GameObject<CStageSwapCube>(true);
	m_pStageSwapCube4->SetShadow(this, VEC3EX(3,4,0), m_pShadowLight);
	m_pStageSwapCube4->SetPlayer(pPlayer);
	m_pStageSwapCube4->SetHide(true,true);
	m_pStageSwapCube4->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA1", RESOURCE_STATIC, "TEX_SEA_BGA3");
	m_pStageSwapCube3->AddCube(m_pStageSwapCube4);
	m_pStageSwapCube4->AddCube(m_pStageSwapCube3);

	RCP_BASICCUBE pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetHide(false, true);
	m_pStageSwapCube1->AddCube(pBasicCube);

	RCP_MOVECUBE pMoveCube = pLayer->Add_GameObject<CMoveCube>(true);
	pMoveCube->Setting(this, VEC3EX(1,0,0), VEC3EX(1,2,0), 2, 1, m_pShadowLight);
	pMoveCube->SetHide(false, true);
	m_pStageSwapCube1->AddCube(pMoveCube);

	for(int i = 0 ; i < 3; i++)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(-i+2, i+2, 0), m_pShadowLight);
		pBasicCube->SetHide(true, true);
		m_pStageSwapCube1->AddCube(pBasicCube);
		m_pStageSwapCube2->AddCube(pBasicCube);
	}

	RCP_BRIDGECUBE pBridgeCube = pLayer->Add_GameObject<CBridgeCube>(true); 
	pBridgeCube->SetShadow(this, VEC3EX(0,2,6), m_pShadowLight); 
	pBridgeCube->SetHide(true, true);
	pBridgeCube->SetViewDir(DIR_RT); 
	pBridgeCube->SetPlayer(pPlayer);
	m_pStageSwapCube2->AddCube(pBridgeCube);
	m_pStageSwapCube4->AddCube(pBridgeCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(_vec3(VEC_LB(1)+VEC_RB(1)));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube2->AddCube(pBasicCube);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(_vec3(VEC_LB(2)+VEC_RB(2)));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube2->AddCube(pBasicCube);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pMoveCube = pLayer->Add_GameObject<CMoveCube>(true);
	pMoveCube->Setting(this, VEC3EX(0,2,2), VEC3EX(2,4,0), 2, 1, m_pShadowLight);
	pMoveCube->SetHide(true, true);
	m_pStageSwapCube3->AddCube(pMoveCube);
	m_pStageSwapCube4->AddCube(pMoveCube);

	pBridgeCube = pLayer->Add_GameObject<CBridgeCube>(true); 
	pBridgeCube->SetShadow(this, VEC3EX(5,4,0), m_pShadowLight); 
	pBridgeCube->SetHide(true, true);
	pBridgeCube->SetViewDir(DIR_RT); 
	pBridgeCube->SetPlayer(pPlayer);
	m_pStageSwapCube4->AddCube(pBridgeCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(1,0,1));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(0,-1,2));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(-1,-2,1));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pBridgeCube = pLayer->Add_GameObject<CBridgeCube>(true); 
	pBridgeCube->SetShadow(this, VEC3EX(2, 1, 0), m_pShadowLight); 
	pBridgeCube->SetHide(true, true);
	pBridgeCube->SetViewDir(DIR_LT); 
	pBridgeCube->SetPlayer(pPlayer);
	m_pStageSwapCube4->AddCube(pBridgeCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(1, 0, 1));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(2, 0, 2));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	pBasicCube->SetHide(true, true);
	m_pStageSwapCube4->AddCube(pBasicCube);

	pMoveCube = pLayer->Add_GameObject<CMoveCube>(true);
	pMoveCube->Setting(this, VEC3EX(2, 2.f, -3.8f), VEC3EX(-3, 2.f, 0), 2, 1, m_pShadowLight);
	pMoveCube->SetHide(true, true);
	m_pStageSwapCube4->AddCube(pMoveCube);

	pLayer->Add_GameObject<CNextCube>(true)->Setting(this, VEC3EX(-3,2,1), pPlayer, "CStage06_2", m_pFadeInOutFilter, m_pShadowLight);
	
	RCP_OCEAN pOcean;
	pOcean = pLayer->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));
	
	InitBubble();
}

void CStage06_1::Disable()
{
	this->Clear();
}

void CStage06_1::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Wall", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");

	CStageScene::ClickEvent();

	RegenBubble();
}
