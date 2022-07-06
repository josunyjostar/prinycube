#include "stdafx.h"
#include "Stage07_2.h"
#include "Stage08.h"

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
#include "Ray.h"
#include "MirrorCube.h"
#include "ColorCube.h"

#include "Ocean.h"
#include "BackGround.h"

#include "Light.h"
#include "Dust.h"

#include "ToolTip.h"

#include "Fountain.h"
#include "Dead.h"
#include "DeadWater.h"

#include "BTMenu.h"

CStage07_2::CStage07_2()
{
}

CStage07_2::~CStage07_2()
{
}


void CStage07_2::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 7;
	Engine::CSceneManager::Inst().CreateScene<CStage08>();

	Engine::SoundMgr->Play(SND_LASER);

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

	//스타트 큐브
	RCP_STAGESWAPCUBE pStageSwapCube1 = pLayer->Add_GameObject<CStageSwapCube>(true);
	pStageSwapCube1->SetShadow(this, VEC3EX(-2,0,0), m_pShadowLight);
	pStageSwapCube1->SetPlayer(pPlayer);
	pStageSwapCube1->SetHide(false,false);
	pStageSwapCube1->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_SEA_BGA4", RESOURCE_STATIC, "TEX_SEA_BGA5");

	RCP_BASICCUBE pBasicCube;

	int ControlPos = -1;
	//컬러 큐브 와 클리어 길
	RCP_COLORCUBE pColorCube = pLayer->Add_GameObject<CColorCube>(true);
	pColorCube->SetShadow(this, VEC3EX(-2,6+ControlPos,0), m_pShadowLight);

	for(int i = 0 ; i < 3; ++i)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(0,-2,-5-i), m_pShadowLight);
		pBasicCube->SetHide(true,true);
		pColorCube->AddCube(pBasicCube);
	}

	for(int i = 1 ; i < 7; ++i)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(0+i,-2,-7), m_pShadowLight);
		pBasicCube->SetHide(true,true);
		pColorCube->AddCube(pBasicCube);
	}

	for(int i = 1 ; i < 5; ++i)
	{
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(6,-2+i*0.5f,-7+i), m_pShadowLight);
		pBasicCube->SetHide(true,true);
		pColorCube->AddCube(pBasicCube);
	}
	//
	//바닥 
	for(int i=0; i < 10; ++i)
	{	
		for(int j=0; j < 10; ++j)
		{	
			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(-5+j,-1,-4+i), m_pShadowLight);
		}
	}
	//
	RCP_LAYER pRayLayer = this->Create_Layer("Ray");
	RCP_RAYDRAW drawray;
	drawray = pRayLayer->Add_GameObject<CRayDrawing>(true);
	RCP_RAYOBJ ray;
	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(3.85f,0.f,5), VEC3EX(0,0,-1));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(1.f, 0.f, 0.f));

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(4,0,5), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	//미러
	RCP_LAYER& pMirrorLayer = this->Create_Layer("Mirror");

	RCP_MIRRORCUBE pMirrorCube;						//우
	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(7,0,4), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI);
	//pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);//좌
	pMirrorCube->SetShadow(this, VEC3EX(-6,0,-6), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), 0.f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //하
	pMirrorCube->SetShadow(this, VEC3EX(7,0,-6), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI * 0.5f); 

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);//상
	pMirrorCube->SetShadow(this, VEC3EX(-6,0,4), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI * 0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);//청픽업 좌상반사
	pMirrorCube->SetShadow(this, VEC3EX(4,0,-4), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(0.f,0.f, 1.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), 0.f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);//청픽업 우하로 반사
	pMirrorCube->SetShadow(this, VEC3EX(2,0,-4), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(0.f,0.f, 1.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI);

	//pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);//청픽업 좌하 
	//pMirrorCube->SetShadow(this, VEC3EX(-2,0,3), m_pShadowLight);
	//pMirrorCube->SetHide(false, false);
	//pMirrorCube->SetCanPick(true);
	//pMirrorCube->SetColor(_vec3(1.f,0.f, 0.f));
	//pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	//pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);

	//pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);//청픽업 좌하 
	//pMirrorCube->SetShadow(this, VEC3EX(-2,0,3), m_pShadowLight);
	//pMirrorCube->SetHide(false, false);
	//pMirrorCube->SetCanPick(true);
	//pMirrorCube->SetColor(_vec3(1.f,0.f, 0.f));
	//pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	//pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI*0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //적레이 상반사 큐브1
	pMirrorCube->SetShadow(this, VEC3EX(2,0,-7), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI * 0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //적레이 상반사 큐브2
	pMirrorCube->SetShadow(this, VEC3EX(2,6+ControlPos,-7), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI * 0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), -PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //공중 미러
	pMirrorCube->SetShadow(this, VEC3EX(2,6+ControlPos,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(206/255,242/255, 121/255));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI);

	//녹색 레이저 차례

	pRayLayer = this->Create_Layer("Ray");
	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(0,5+ControlPos,0), VEC3EX(1,0,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(0.f, 1.f, 0.f));

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-0.25f,5+ControlPos,0), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //공중 미러
	pMirrorCube->SetShadow(this, VEC3EX(4,5+ControlPos,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(false);
	pMirrorCube->SetColor(_vec3(0.f,0.f, 0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);
	pTransform->Rotate(pTransform->GetUpVector(), -PI);

	//pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //옐로 미러 우상
	//pMirrorCube->SetShadow(this, VEC3EX(-4,0,-2), m_pShadowLight);
	//pMirrorCube->SetHide(false, false);
	//pMirrorCube->SetCanPick(true);
	//pMirrorCube->SetColor(_vec3(1.f,1.f, 0.f));
	//pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	//pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI * 0.25f);
	//pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //옐로 미러 좌상
	pMirrorCube->SetShadow(this, VEC3EX(-5,0,-3), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(1.f,1.f, 0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI * 0.75f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //옐로 미러 우하
	pMirrorCube->SetShadow(this, VEC3EX(-3,0,3), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(1.f,1.f, 0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI * 0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //옐로 미러 우하
	pMirrorCube->SetShadow(this, VEC3EX(4,0,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(1.f,1.f, 0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI * 0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	//pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //옐로 미러 좌하
	//pMirrorCube->SetShadow(this, VEC3EX(-4,0,-4), m_pShadowLight);
	//pMirrorCube->SetHide(false, false);
	//pMirrorCube->SetCanPick(true);
	//pMirrorCube->SetColor(_vec3(1.f,1.f, 0.f));
	//pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	//pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI * 0.75f);
	//pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //자주색 
	pMirrorCube->SetShadow(this, VEC3EX(-5,0,5), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(1.f,0.f,1.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI * 0.75f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), -PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //공중 미러
	pMirrorCube->SetShadow(this, VEC3EX(-6,2,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(0.f,0.f, 0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI * 0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); //공중미러
	pMirrorCube->SetShadow(this, VEC3EX(-6,5,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(0.f,0.f,0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI * 0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), -PI*0.25f);

	//파란 레이저 차례

	pRayLayer = this->Create_Layer("Ray");
	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(-2,8,0), VEC3EX(0,-1,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(0.f, 0.f, 1.f));

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-2,8.25f,0), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	////  클리어 ///////////////////////////////////////////////////////////
	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(6,0.5f,-2), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(6,1.f,-2), pPlayer, "CStage08", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC3EX(0,1.5f,5), m_pShadowLight);

	//------------------------------------------------------------------------------------------------------------------
	RCP_OCEAN pOcean;
	pOcean = this->Create_Layer("Tile")->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));

	InitSeaFood();
}
	
void CStage07_2::Disable()
{
	Engine::SoundMgr->Stop(SND_LASER);
	this->Clear();
}

void CStage07_2::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Light");

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Wall", "Arrow");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Tile", "Arrow");

	CStageScene::ClickEvent();

		RegenSeaFood();

	RCP_LAYER pLayer = this->Find_Layer("Tile");
	RCP_COLORCUBE pColorCube = pLayer->Find_GameObject("CColorCube");

	if(pColorCube->GetColor() == _vec3(1.f, 1.f, 1.f))
	{
		pColorCube->StageSwap(false);
	}
}
