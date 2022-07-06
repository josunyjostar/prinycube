#include "stdafx.h"
#include "Stage09.h"
#include "EndingScene.h"

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
#include "ColorCube.h"
#include "PickUpCube.h"
#include "MirrorCube.h"
#include "Ray.h"
#include "StagePopup.h"

#include "BTMenu.h"

CStage09::CStage09()
{
}

CStage09::~CStage09()
{
}

void CStage09::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_GAME))
	{
		Engine::SoundMgr->AllStop();
		Engine::SoundMgr->Play(BGM_GAME);
	}

	g_GameData.RunningStage = 9;
	Engine::CSceneManager::Inst().CreateScene<CEndingScene>();

	m_pShadowLight = this->Create_Layer("ShadowLight")->Add_GameObject<CShadowLight>(true);

	Engine::SoundMgr->Play(SND_LASER);

	RCP_LIGHT pLight = Engine::CLightManager::Inst().GetLight("DirectionLight");
	pLight->DeviceInLight();

	m_pDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	RCP_CAMERA& pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(m_matView);
	pMainCam->SetProjMatrix(m_matProj);

	RCP_PLAYER& pPlayer = this->Create_Layer("Player")->Add_GameObject<CPlayer>(true);
	pPlayer->GetTransform()->SetPosition(VEC3EX(0, 5, 0));
	pPlayer->SetSpawnPos(VEC3EX(0, 5, 0));
	pPlayer->SetShadow(this, m_pShadowLight);

	RCP_TRANSFORM pTransform;
	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_BACKGROUND& pBackGround = pUILayer->Add_GameObject<CBackGround>(true);
	pBackGround->SetTexture(RESOURCE_STATIC, "TEX_SEA_BGA5");
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

	RCP_LAYER pMirrorLayer = this->Create_Layer("Mirror");

	RCP_MIRRORCUBE pMirrorCube;

	RCP_LAYER pRayLayer = this->Create_Layer("Ray");
	RCP_RAYDRAW drawray;
	drawray = pRayLayer->Add_GameObject<CRayDrawing>(true);
	RCP_RAYOBJ ray;

	for(int i = 0 ; i < 9; i++)
	{
		for(int j = 0 ; j < 9; j++)
		{
			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(-3+i, -1.5, -3+j), m_pShadowLight);
		}
	}

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,-1,-2), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(-4.85,-1,-2), VEC3EX(1,0,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(1.f, 0.f, 0.f));


	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,-1,0), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(-4.85,-1,0), VEC3EX(1,0,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(0.f, 1.f, 0.f));


	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,-1,2), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	ray = pRayLayer->Add_GameObject<CRay>(true);
	ray->SetStartRay(VEC3EX(-4.85,-1,2), VEC3EX(1,0,0));
	ray->SetDrawRay(drawray);
	ray->SetColor(_vec3(0.f, 0.f, 1.f));

	{	
		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(-2,-1,4), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(-2,-1,5), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(-1,-1,5), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(0,-1,4), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.0f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(0,-1,5), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.0f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(1,-1,5), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(1,-1,4), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(2,-1,5), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);
		pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);

		pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
		pMirrorCube->SetShadow(this, VEC3EX(3,-1,5), m_pShadowLight);
		pMirrorCube->SetCanPick(true);
		pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
		pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
		pTransform->SetScaleY(0.5f);




	}


	//R


	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-2,-1,3), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(2,-1,-2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(3,-1,-1), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);





	//G
	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-3,-1,0), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-3,-1,-3), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);



	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(0,-1,-2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-1,-1,-2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(0,-1,-1), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(2,-1,2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.0f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(2,-1,0), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.f);


	//B
	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-3,-1,2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-1,-1,1), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.0f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(-1,-1,0), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(1,-1,-3), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);


	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(4,-1,2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);



	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(6,-1,-2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);
	pTransform->Rotate(pTransform->GetLookVector(), PI*0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(6,-1,0), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);
	pTransform->Rotate(pTransform->GetLookVector(), PI*0.5f);

	pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube->SetShadow(this, VEC3EX(6,-1,2), m_pShadowLight);
	pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);
	pTransform->Rotate(pTransform->GetLookVector(), PI*0.5f);



	pColorCube1 = pLayer->Add_GameObject<CColorCube>(true);
	pColorCube1->SetShadow(this, VEC3EX(6,2,0), m_pShadowLight);

	pMirrorCube1 = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube1->SetShadow(this, VEC3EX(6,2,-2), m_pShadowLight);
	pTransform = pMirrorCube1->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(pTransform->GetLookVector(), PI*-0.5f);

	pMirrorCube2 = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
	pMirrorCube2->SetShadow(this, VEC3EX(6,2,2), m_pShadowLight);
	pTransform = pMirrorCube2->GetComponent<Engine::CTransform>();
	pTransform->SetScaleY(0.5f);
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.f);
	pTransform->Rotate(pTransform->GetLookVector(), PI*-0.5f);
	pColorCube1->AddCube(pMirrorCube2);


	for(int i = 0 ; i < 5; i++)
	{
		for(int j = 0 ; j < 8; j++)
		{
			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(-3+i, 1.5, -2+j), m_pShadowLight);
			pBasicCube->SetHide(true, true);
			pColorCube1->AddCube(pBasicCube);
		}
	}

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,2,-2), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));
	pColorCube1->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,2,0), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));
	pColorCube1->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,2,2), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));
	pColorCube1->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(2,-0.5,-4), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pColorCube1->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(1, 0.5,-4), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pColorCube1->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0, 1.5,-4), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pColorCube1->AddCube(pBasicCube);

	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0, 1.5,-3), m_pShadowLight);
	pBasicCube->SetHide(true, true);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pColorCube1->AddCube(pBasicCube);


	pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(-5,1,-2), m_pShadowLight);
	pTransform = pBasicCube->GetComponent<Engine::CTransform>();
	pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));





	RCP_OCEAN pOcean;
	pOcean = pLayer->Add_GameObject<COcean>(true);
	pTransform = pOcean->GetComponent<Engine::CTransform>();
	pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
	pTransform->AddPosition(_vec3(0.f, -5.f, 1.5f));

	pBasicCube = pLayer->Add_GameObject<CTempleCube>(true);
	pBasicCube->SetShadow(this, VEC3EX(0, 4.5, -5), m_pShadowLight);
	pLayer = this->Create_Layer("Light");
	RCP_TEMPLELIGHT pTempleLight = pLayer->Add_GameObject<CTempleLight>(true);
	pTempleLight->Setting(this,  VEC3EX(0, 5.5, -5), pPlayer, "CEndingScene", m_pFadeInOutFilter, m_pShadowLight);
	pLayer->Add_GameObject<CTempleLightSource>(true)->SetShadow(this, VEC3EX(0, 5.5, -5), m_pShadowLight);

	InitSeaFood();
}

void CStage09::Disable()
{
	Engine::SoundMgr->Stop(SND_LASER);
	this->Clear();
}

void CStage09::Update()
{
	CStageScene::Update();

	if(m_fDeltaTime == 0.f) return;

	static _bool bState = false;

#if defined(_DEBUG) || defined(DEBUG)
	if(Engine::KeyInput->IsDown('A'))
		this->Find_Layer("Player")->Find_GameObject("CPlayer")->GetComponent<Engine::CTransform>()->SetPosition(VEC3EX(0, 5.5, -5));
#endif

	if(!bState)
	{
		if(pColorCube1->GetColor() == _vec3(1.f, 1.f, 1.f))
		{
			bState = true;
			pColorCube1->StageSwap();
			pColorCube1->SetActive(false);
			pMirrorCube1->SetActive(false);
			pMirrorCube2->SetActive(false);
			RCP_BASICCUBE pBasicCube;
			RCP_TRANSFORM pTransform;
			RCP_LAYER pRayLayer = this->Create_Layer("Ray");
			RCP_RAYDRAW drawray;
			drawray = pRayLayer->Add_GameObject<CRayDrawing>(true);
			RCP_RAYOBJ ray;
			ray = pRayLayer->Add_GameObject<CRay>(true);
			ray->SetStartRay(VEC3EX(-4.85,2,-2), VEC3EX(1,0,0));
			ray->SetDrawRay(drawray);
			ray->SetColor(_vec3(1.f, 1.f, 0.f));
			ray = pRayLayer->Add_GameObject<CRay>(true);
			ray->SetStartRay(VEC3EX(-4.85,2,0), VEC3EX(1,0,0));
			ray->SetDrawRay(drawray);
			ray->SetColor(_vec3(0.f, 1.f, 1.f));
			ray = pRayLayer->Add_GameObject<CRay>(true);
			ray->SetStartRay(VEC3EX(-4.85,2,2), VEC3EX(1,0,0));
			ray->SetDrawRay(drawray);
			ray->SetColor(_vec3(1.f, 0.f, 1.f));
			RCP_LAYER pLayer = this->Create_Layer("Tile");
			pColorCube2 = pLayer->Add_GameObject<CColorCube>(true);
			pColorCube2->SetShadow(this, VEC3EX(6,2,-2), m_pShadowLight);
			pColorCube3 = pLayer->Add_GameObject<CColorCube>(true);
			pColorCube3->SetShadow(this, VEC3EX(6,2,0), m_pShadowLight);
			pColorCube4 = pLayer->Add_GameObject<CColorCube>(true);
			pColorCube4->SetShadow(this, VEC3EX(6,2,2), m_pShadowLight);

			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(1, 2.5,-3), m_pShadowLight);
			pBasicCube->SetHide(true, true);
			pTransform = pBasicCube->GetComponent<Engine::CTransform>();
			pColorCube2->AddCube(pBasicCube);
			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(1, 3.5,-4), m_pShadowLight);
			pBasicCube->SetHide(true, true);
			pTransform = pBasicCube->GetComponent<Engine::CTransform>();
			pColorCube3->AddCube(pBasicCube);
			pBasicCube = pLayer->Add_GameObject<CBasicCube>(true);
			pBasicCube->SetShadow(this, VEC3EX(1, 4.5,-5), m_pShadowLight);
			pBasicCube->SetHide(true, true);
			pTransform = pBasicCube->GetComponent<Engine::CTransform>();
			pColorCube4->AddCube(pBasicCube);


			RCP_LAYER pMirrorLayer = this->Create_Layer("Mirror");
			RCP_MIRRORCUBE pMirrorCube;

			pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); 
			pMirrorCube->SetShadow(this, VEC3EX(-3,2,3), m_pShadowLight);
			pMirrorCube->SetCanPick(true);
			pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
			pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
			pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.5f);
			pTransform->SetScaleY(0.5f);
			pColorCube1->AddCube(pMirrorCube);

			pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true); 
			pMirrorCube->SetShadow(this, VEC3EX(-1,2,3), m_pShadowLight);
			pMirrorCube->SetCanPick(true);
			pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
			pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
			pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*-0.5f);
			pTransform->SetScaleY(0.5f);
			pColorCube1->AddCube(pMirrorCube);


			for(int i = 0 ; i < 3; i++)
			{
				pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
				pMirrorCube->SetShadow(this, VEC3EX(-2,2,3+i), m_pShadowLight);
				pMirrorCube->SetCanPick(true);
				pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
				pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
				pTransform->SetScaleY(0.5f);
				pColorCube1->AddCube(pMirrorCube);

				pMirrorCube = pMirrorLayer->Add_GameObject<CMirrorCube>(true);
				pMirrorCube->SetShadow(this, VEC3EX(0,2,3+i), m_pShadowLight);
				pMirrorCube->SetCanPick(true);
				pMirrorCube->SetColor(_vec3(0.8f, 0.8f, 0.f));
				pTransform = pMirrorCube->GetComponent<Engine::CTransform>();
				pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*1.f);
				pTransform->SetScaleY(0.5f);
				pColorCube1->AddCube(pMirrorCube);
			}
		}
	}
	else
	{
		if(pColorCube2->GetColor() == _vec3(1.f, 1.f, 1.f))
			pColorCube2->StageSwap(false);
		else
			pColorCube2->StageSwap(true);

		if(pColorCube3->GetColor() == _vec3(1.f, 1.f, 1.f))
			pColorCube3->StageSwap(false);
		else
			pColorCube3->StageSwap(true);

		if(pColorCube4->GetColor() == _vec3(1.f, 1.f, 1.f))
			pColorCube4->StageSwap(false);
		else
			pColorCube4->StageSwap(true);

	}

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");

	CStageScene::ClickEvent();

	RegenSeaFood();
}