#include "stdafx.h"
#include "Stage02.h"
#include "Stage03.h"

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

CStage02::CStage02()
{

}

CStage02::~CStage02()
{
}

void CStage02::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 2;
	Engine::CSceneManager::Inst().CreateScene<CStage03>();

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
	pBackGround->SetTexture(RESOURCE_STATIC, "TEX_SEA_BGA2");
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

	pPlayer->GetTransform()->SetPosition(VEC_LT(2)+VEC_Y(1));
	RCP_ARROWTRAPCUBE ArrowTrapCube;
	RCP_BASICCUBE pBasicCube;
	RCP_OCEAN pOcean;

	pLayer = this->Create_Layer("Tile");
	for(int i = 0 ; i < 3; i++)
		pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC_LT(i*1.1f), m_pShadowLight);

	for(int i = 1 ; i < 5; i++)
		pLayer->Add_GameObject<CBasicCube>(true)->SetShadow(this, VEC_RB(5.1f)+VEC_LB(i*1.1f)+VEC_Y(2), m_pShadowLight);

	pLayer->Add_GameObject<CMoveCube>(true)->Setting(this, VEC_RB(1.1f), VEC_RB(4)+VEC_LB(4.4f)+VEC_Y(2), 1, 1.5f, m_pShadowLight);

	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(2, -1, 0), DIR_LT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(2, -1, 0), DIR_LT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(3, -2, -3), DIR_LT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(5, 5, -2), DIR_LT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(5, 4, -5), DIR_LT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(2, -1, 3), DIR_LB, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(0, 5, 4), DIR_LB, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(0, 5, -3), DIR_RT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(1, -1, -5), DIR_RT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-1, -2, -4), DIR_RT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(3, 5, -3), DIR_RT, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC3EX(-5, 5, 0), DIR_RB, RAND_RANGE_F(0.5f, 0.25f), m_pShadowLight);
	pLayer->Add_GameObject<CArrowTrapCube>(true)->Setting(this, VEC_RB(7.2f)+VEC_LB(1.1f)+VEC_Y(3), DIR_LT, 3.f, m_pShadowLight);

	pOcean = pLayer->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC_RB(6.2f)+VEC_LB(1.1f)+VEC_Y(2), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC_RB(6.2f)+VEC_LB(1.1f)+VEC_Y(3), pPlayer, "CStage03", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC_RB(6.2f)+VEC_LB(1.1f)+VEC_Y(3), m_pShadowLight);
}	

void CStage02::Disable()
{
	this->Clear();
}

void CStage02::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Wall", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Tile", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");

	CStageScene::ClickEvent();
}
