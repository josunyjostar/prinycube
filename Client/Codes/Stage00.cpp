#include "stdafx.h"
#include "Stage00.h"
#include "Stage01.h"

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

CStage00::CStage00()
{

}

CStage00::~CStage00()
{
}

void CStage00::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 0;
	Engine::CSceneManager::Inst().CreateScene<CStage01>();

	m_pShadowLight = this->Create_Layer("ShadowLight")->Add_GameObject<CShadowLight>(true);

	RCP_LIGHT pLight = Engine::CLightManager::Inst().GetLight("DirectionLight");
	pLight->DeviceInLight();

	m_pDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	RCP_CAMERA& pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(m_matView);
	pMainCam->SetProjMatrix(m_matProj);

	RCP_PLAYER& pPlayer = this->Create_Layer("Player")->Add_GameObject<CPlayer>(true);
	pPlayer->GetTransform()->SetPosition(VEC_RB(4)+VEC_Y(1));
	pPlayer->SetSpawnPos(VEC_RB(4)+VEC_Y(1));
	pPlayer->SetShadow(this, m_pShadowLight);

	RCP_TRANSFORM pTransform;
	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_BACKGROUND& pBackGround = pUILayer->Add_GameObject<CBackGround>(true);
	pBackGround->SetTexture(RESOURCE_STATIC, "TEX_DAY");
	pUILayer->Add_GameObject<CBTMenu>(true);
	m_pFadeInOutFilter = pUILayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->SetPosition(pPlayer->GetTransform()->GetScreenPos(m_pDev, WINCX, WINCY));
	m_pFadeInOutFilter->FadeOut(3.f, FADE_OUT);
	m_pFadeInOutFilter->SetColor(_vec4(1,1,1,1));
	m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));

	RCP_STAGEPOPUP pStagePopup = pUILayer->Add_GameObject<CStagePopup>();

	RCP_LAYER& pLayer = this->Create_Layer("Wall");

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
	for(int i = 0 ; i < 5; i++)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC_RB(i), m_pShadowLight);
	}

	for(int i = 0 ; i < 5; i++)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC_LT(1.2f)+VEC_LB(i*1.2f), m_pShadowLight);
	}

	RCP_OCEAN pOcean;
	pOcean = pLayer->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC_LT(1.2f)+VEC_LB(5*1.2f), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this,  VEC_LT(1.2f)+VEC_LB(5*1.2f) + VEC_Y(1), pPlayer, "CStage01", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC_LT(1.2f)+VEC_LB(5*1.2f) + VEC_Y(1), m_pShadowLight);
}	

void CStage00::Disable()
{
	this->Clear();
}

void CStage00::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");

	CStageScene::ClickEvent();
}