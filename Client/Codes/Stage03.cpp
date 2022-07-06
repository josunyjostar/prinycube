#include "stdafx.h"
#include "Stage03.h"
#include "Stage04.h"

#include "Player.h"
#include "SkyBox.h"
#include "Wall.h"
#include "BasicCube.h"
#include "MoveCube.h"
#include "Ocean.h"
#include "BridgeCube.h"
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
#include "BTMenu.h"
#include "StagePopup.h"

CStage03::CStage03()
{

}

CStage03::~CStage03()
{
}

void CStage03::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 3;
	Engine::CSceneManager::Inst().CreateScene<CStage04>();

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

	RCP_TRANSFORM pTransform;
	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_BACKGROUND& pBackGround = pUILayer->Add_GameObject<CBackGround>(true);
	pBackGround->SetTexture(RESOURCE_STATIC, "TEX_SKY_BGA2");
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
	RCP_BASICCUBE pBasicCube;

	RCP_STAGESWAPCUBE pStageSwapCube1 = pLayer->Add_GameObject<CStageSwapCube>(true);
	pStageSwapCube1->SetShadow(this, VEC3EX(-4, 3, 0), m_pShadowLight);
	pStageSwapCube1->SetPlayer(pPlayer);
	pStageSwapCube1->SetHide(false,true);
	pStageSwapCube1->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SKY_BGA2", RESOURCE_STATIC, "TEX_SKY_BGA3");

	RCP_TOOLTIPTRIGGER pToolTip = pLayer->Add_GameObject<CToolTipTrigger>(true);
	pToolTip->SetPosition(VEC3EX(-4, 3, 0));
	pToolTip->SetToolTip(RESOURCE_STATIC, "TEX_F_TOOLTIP_ACTIVE", 4.f, 0.5f, 0.5f, _vec2(256.f, 80.f), _vec2(120.f, -50.f));

	RCP_MOVECUBE pMoveCube = pLayer->Add_GameObject<CMoveCube>(true);
	pMoveCube->Setting(this, VEC3EX(-3, 0, 0), VEC3EX(-3, 3, 0), 1, 1.5f, m_pShadowLight);
	pMoveCube->SetHide(false, true);
	pStageSwapCube1->AddCube(pMoveCube);

	pMoveCube = pLayer->Add_GameObject<CMoveCube>(true);
	pMoveCube->Setting(this, VEC3EX(1, 0, 0), VEC3EX(3, 2, -3), 1, 1.5f, m_pShadowLight);
	pMoveCube->SetHide(true, true);
	pStageSwapCube1->AddCube(pMoveCube);

	for(int i = 0 ; i < 3; i++)
		pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC3EX(i-2, 0, 0), m_pShadowLight);

	for(int i = 1 ; i < 5; i++)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(5, 3, -i), m_pShadowLight);
		pBasicCube->SetHide(true, true);
		pStageSwapCube1->AddCube(pBasicCube);
	}
	
	RCP_BRIDGECUBE pBridgeCube = pLayer->Add_GameObject<CBridgeCube>(true);
	pBridgeCube->SetShadow(this, VEC3EX(2, 1, 0), m_pShadowLight); 
	pBridgeCube->SetViewDir(DIR_RT); 
	pBridgeCube->SetPlayer(pPlayer);

	for(int i = 0 ; i < 3; i++)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
		pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
		pBasicCube->SetLocalPos(_vec3(i*1.1f+1.1f, (float)i, 0));
		pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());
	}

	RCP_OCEAN pOcean = pLayer->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));
	
	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(5, 3, -5), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(5, 4, -5), pPlayer, "CStage04", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC3EX(5, 4, -5), m_pShadowLight);
}	

void CStage03::Disable()
{
	this->Clear();
}

void CStage03::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Wall", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");

	CStageScene::ClickEvent();
}
