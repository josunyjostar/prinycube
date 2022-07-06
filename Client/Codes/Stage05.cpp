#include "stdafx.h"
#include "Stage05.h"
#include "Stage06_1.h"

#include "Player.h"
#include "SkyBox.h"
#include "Wall.h"

#include "BasicCube.h"
#include "MoveCube.h"
#include "BridgeCube.h"
#include "TempleCube.h"
#include "NextCube.h"
#include "StageSwapCube.h"
#include "ColorCube.h"
#include "PickUpCube.h"
#include "MirrorCube.h"
#include "Ray.h"

#include "Ocean.h"

#include "BackGround.h"

#include "Light.h"
#include "Dust.h"

#include "ToolTip.h"

#include "Fountain.h"
#include "Dead.h"
#include "DeadWater.h"
#include "StagePopup.h"

#include "BTMenu.h"

CStage05::CStage05()
{
}

CStage05::~CStage05()
{
}

void CStage05::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 5;
	Engine::CSceneManager::Inst().CreateScene<CStage06_1>();

	Engine::SoundMgr->Play(SND_LASER);

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
	pPlayer->GetTransform()->SetPosition(VEC3EX(-5, 1, -5));
	pPlayer->SetSpawnPos(VEC3EX(-5, 1, -5));

	RCP_TRANSFORM pTransform;
	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_BACKGROUND& pBackGround = pUILayer->Add_GameObject<CBackGround>(true);
	pBackGround->SetTexture(RESOURCE_STATIC, "TEX_NIGHT");
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

	//RCP_STAGESWAPCUBE pStageSwapCube1 = pLayer->Add_GameObject<CStageSwapCube>(true);
	//pStageSwapCube1->SetShadow(this, VEC3EX(-5,-2,-2), m_pShadowLight);
	//pStageSwapCube1->SetPlayer(pPlayer);
	//pStageSwapCube1->SetHide(false,false);
	//pStageSwapCube1->SetBackGround(pBackGround, RESOURCE_STATIC, "TEX_DAY", RESOURCE_STATIC, "TEX_NIGHT");

	//RCP_PICKUPCUBE pPickCube = this->Create_Layer("PickCube")->Add_GameObject<CPickUpCube>(true);
	//pPickCube->SetShadow(this, VEC3EX(-5,-1,-3) , m_pShadowLight);
	//pPickCube->SetHide(true,true);
	//pStageSwapCube1->AddCube(pPickCube);

	RCP_BASICCUBE pBasicCube;// START CUBE POS
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,0.25,-5), m_pShadowLight);



	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,1,0), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));


	RCP_LAYER pRayLayer = this->Create_Layer("Ray");
	RCP_RAYDRAW drawray;
	drawray = pRayLayer->Add_GameObject<CRayDrawing>(true);
	RCP_RAYOBJ ray;
	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(-4.85,1,0), VEC3EX(1,0,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(1.f, 0.f, 0.f));



	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(5,6,0), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	pRayLayer = this->Create_Layer("Ray");
	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(4.85,6,0), VEC3EX(-1,0,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(0.f, 1.f, 0.f));



	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(4,2,4), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	pRayLayer = this->Create_Layer("Ray");
	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(4,1.5,4), VEC3EX(0,-1,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(0.f, 0.f, 1.f));



	pColorCube1 = pLayer->Add_GameObject<CColorCube>(true);
	pColorCube1->SetShadow(this, VEC3EX(-0,1,-0), m_pShadowLight);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0,1,3), m_pShadowLight);
	pColorCube1->AddCube(pBasicCube);
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0,1,4), m_pShadowLight);
	pColorCube1->AddCube(pBasicCube);


	for(int i = 0 ; i < 5 ;++i)
	{		
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(-4+i,0.25,-5), m_pShadowLight);
	}
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0,0.25,-5), m_pShadowLight);
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0,0.25,-4), m_pShadowLight);
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0,0.25,-3), m_pShadowLight);
	for(int i = 0 ; i < 5 ;++i)
	{		
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(-2+i,0.25,-2), m_pShadowLight);
	}
	for(int i = 0 ; i < 4 ;++i)
	{		
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(-2,0.25,-1+i), m_pShadowLight);
	}
	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX( 3,0.25,0), m_pShadowLight);
	for(int i = 0 ; i < 3 ;++i)
	{		
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX( 2,0.25,-1+i), m_pShadowLight);
		pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
		pBasicCube->SetShadow(this, VEC3EX(-1+i,0.25, 2), m_pShadowLight);
	}

	RCP_BRIDGECUBE pBridgeCube = pLayer->Add_GameObject<CBridgeCube>(true);
	pBridgeCube->SetShadow(this, VEC3EX(2, 0.25, 2), m_pShadowLight); 
	pBridgeCube->SetViewDir(DIR_RT); 
	pBridgeCube->SetPlayer(pPlayer);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(0, -3, 3));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(3, -3, 0));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(0, -3, -3));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, _vec3(), m_pShadowLight);
	pBasicCube->SetLocalScale(_vec3(1.f, 0.5f, 1.f));
	pBasicCube->SetLocalPos(VEC3EX(-3, -3, 0));
	pBasicCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	//for(int i = 0 ; i < 7 ;++i)
	//{
	//	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	//	pBasicCube->SetShadow(this, VEC3EX(1,1,-1+i), m_pShadowLight);

	//}

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0,1,5), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this, VEC3EX(0,2.0f,5), pPlayer, "CStage06_1", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC3EX(0,2.0f,5), m_pShadowLight);


	RCP_LAYER& pMirrorLayer = this->Create_Layer("Mirror");

	RCP_MIRRORCUBE pMirrorCube;
	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-2,1,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetCanPick(true);
	pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.75f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*0.25f);


	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(3,6,0), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->Rotate(pTransform->GetLookVector() - pTransform->GetRightVector(), PI*-0.25f);



	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, _vec3(), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetLocalPos(VEC3EX(3, -2.25, 0));
	pTransform = pBridgeCube->GetComponent<Engine::CTransform>();
	pMirrorCube->SetLocalRotate(pTransform->GetRightVector(), PI*-0.25f);
	pMirrorCube->SetLocalRotate(_vec3(0.f, 1.f, 0.f), PI*-0.75f);
	pMirrorCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, _vec3(), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetLocalPos(VEC3EX(0, -2.25, 3));
	pTransform = pBridgeCube->GetComponent<Engine::CTransform>();
	pMirrorCube->SetLocalRotate(_vec3(0.f, 1.f, 0.f), PI*-0.875f);
	pMirrorCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, _vec3(), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetLocalPos(VEC3EX(-3, -2.25, 0));
	pTransform = pBridgeCube->GetComponent<Engine::CTransform>();
	pMirrorCube->SetLocalRotate(pTransform->GetRightVector(), PI*-0.25f);
	pMirrorCube->SetLocalRotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pMirrorCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, _vec3(), m_pShadowLight);
	pMirrorCube->SetHide(false, false);
	pMirrorCube->SetLocalPos(VEC3EX(0, -2.25, -3));
	pTransform = pBridgeCube->GetComponent<Engine::CTransform>();
	pMirrorCube->SetLocalRotate(_vec3(0.f, 1.f, 0.f), PI*0.125f);
	pMirrorCube->SetParent(pBridgeCube->GetComponent<Engine::CTransform>());





	RCP_OCEAN pOcean;
	pOcean = this->Create_Layer("Tile")->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));
}

void CStage05::Disable()
{
	Engine::SoundMgr->Stop(SND_LASER);
	this->Clear();
}

void CStage05::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	if(pColorCube1->GetColor() == _vec3(1.f, 1.f, 1.f))	pColorCube1->StageSwap(false);
	else												pColorCube1->StageSwap(true);

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "PickCube");
	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Light");

	CStageScene::ClickEvent();
}
