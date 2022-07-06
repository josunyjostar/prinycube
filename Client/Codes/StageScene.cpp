#include "stdafx.h"
#include "StageScene.h"

#include "Ocean.h"
#include "TempleLight.h"
#include "Dust.h"
#include "Dead.h"
#include "Fountain.h"
#include "DeadWater.h"

#include "Fish.h"
#include "JellyFish.h"
#include "Bubble.h"
#include "BubbleEX.h"

CStageScene::CStageScene()
{
	D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 10.f, -20.f), &_vec3(), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixOrthoLH(&m_matProj, 17.5f*WINCX/WINCY, 17.5f, 0.f, 1000.f);
}

CStageScene::~CStageScene()
{
}

HRESULT CStageScene::Init()
{
	m_pTimer = Engine::CTimerManager::Inst().FindTimer("MainApp_Frame");
	m_pDev = Engine::CGraphicDev::Inst().GetDev();	

	const int shadowMapSize = 2048;
	LPDIRECT3DTEXTURE9 TargetTemp;
	LPDIRECT3DSURFACE9 BufferTemp;
	for(int i = 0; i < WALL_END; ++i)
	{
		if (FAILED(m_pDev->CreateTexture(shadowMapSize, shadowMapSize,
			1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
			D3DPOOL_DEFAULT, &TargetTemp, NULL)))
		{
			ERR_MSG("ShadowTexture Created Failed!");
			return E_FAIL;
		}

		if (FAILED(m_pDev->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
			D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
			&BufferTemp, NULL)))
		{
			ERR_MSG("Shadow Depth Surface Created Failed!");
			return E_FAIL;
		}

		m_pShadowRenderTarget[i] = TargetTemp;
		m_pShadowDepthStencil[i] = BufferTemp;
		m_pShadowRenderTarget[i]->Release();
		m_pShadowDepthStencil[i]->Release();
	}

	D3DXMatrixLookAtLH(&m_matLightView[WALL_LEFT], &_vec3(1.f, 0.f, -1.f), &_vec3(0.0f, 0.0f, 0.0f), &_vec3(0.0f, 1.0f, 0.0f));
	D3DXMatrixLookAtLH(&m_matLightView[WALL_RIGHT], &_vec3(-1.f, 0.f, -1.f), &_vec3(0.0f, 0.0f, 0.0f), &_vec3(0.0f, 1.0f, 0.0f));
	D3DXMatrixOrthoLH(&m_matLightProj, 20.48f, 20.48f, 0, 128.f);

	return S_OK;
}

void CStageScene::Update()
{
	Engine::CScene::Update();

	m_fDeltaTime = m_pTimer->GetFrameTime();

	static bool bActinoCam = false;
	if(Engine::KeyInput->IsDown('C'))
	{
		if(bActinoCam) 
		{
			bActinoCam = false;
			RCP_CAMERA pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
			pMainCam->SetViewMatrix(m_matView);
			pMainCam->SetProjMatrix(m_matProj);		
		}
		else
		{
			static _matrix Proj;
			D3DXMatrixPerspectiveFovLH(&Proj, PI/3.f, WINCX/float(WINCY), 0.1f, 1000.f);
			bActinoCam = true;
			RCP_CAMERA pMainCam = Engine::CCameraManager::Inst().SetMainCam("ActionCam");
			pMainCam->SetViewMatrix(m_matView);
			pMainCam->SetProjMatrix(Proj);
		}
	}
}

void CStageScene::ClickEvent()
{
	Engine::COL_INFO ColInfo;
	_vec2 vMousePos = Engine::KeyInput->MousePos();
	_ray rRay = Engine::CCameraManager::Inst().GetMainCam()->GetCamRay(vMousePos);

	ColInfo = Engine::RayCast->Picking(rRay, "Light");
	if(ColInfo.pObj && IsDynamicCast(CTempleLight, ColInfo.pObj)) ((CTempleLight*)ColInfo.pObj)->AddBright(0.075f);

	ColInfo = Engine::RayCast->Picking(rRay, "Tile");
	if(ColInfo.pObj) {
		if(Engine::KeyInput->IsDown(VK_LBUTTON))
		{
			if(IsDynamicCast(COcean, ColInfo.pObj))
			{
				RCP_FOUNTAIN pFountain = this->Create_Layer("EffectLayer")->Add_GameObject<CFountain>(true);
				pFountain->GetComponent<Engine::CTransform>()->SetPosition(ColInfo.vColPos);
			}
			else if(IsDynamicCast(CBasicCube, ColInfo.pObj))
			{
				Engine::SoundMgr->Play(SND_TILE_CLICK);
				RCP_DUST pDust = this->Create_Layer("EffectLayer")->Add_GameObject<CDust>(true);
				pDust->GetComponent<Engine::CTransform>()->SetPosition(ColInfo.vColPos);
			}
		}
	}

	ColInfo = Engine::RayCast->Picking(rRay, "Dead");
	if(ColInfo.pObj && Engine::KeyInput->IsDown(VK_LBUTTON))
		this->Create_Layer("EffectLayer")->Add_GameObject<CDeadWater>()->SetCreatePos(((CDead*)ColInfo.pObj)->GetCreatePos());

#if defined(_DEBUG) || defined(DEBUG)
	if(Engine::KeyInput->IsDown('Q'))
		g_GameData.ClearStage = 9;
#endif
}
void CStageScene::InitSeaFood()
{
	RCP_LAYER pLayerSeaFood = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");
	int RandCnt = rand()%10+5;
	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayerSeaFood->Add_GameObject<CFish>();
		pFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.5f,0.4f,0.f));
		pFish->SetColor(_vec4(0.23f,0.71f,0.8f,RAND_RANGE_F(0.2f,0.6f)));
		pFish->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-10.f,10.f),1.f),CFish::DIR_LEFT,CFish::TYPE_ZIGZAG,90.f);
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_JELLYFISH pJellyFish = pLayerSeaFood->Add_GameObject<CJellyFish>();

		pJellyFish->SetColor(_vec4(0.26f,0.45f,0.85f,RAND_RANGE_F(0.3f,0.6f)));
		pJellyFish->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-11.f,9.f), 0.f));
		pJellyFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}
}

void CStageScene::RegenSeaFood()
{
	static float fFishTime = 2.f;
	fFishTime -= m_fDeltaTime;

	if (fFishTime < 0)
	{
		MakeFish();
		fFishTime = RAND_RANGE_F(4.f,8.f);
	}

	static float fJellyFishTime = 3.f;
	fJellyFishTime -= m_fDeltaTime;

	if (fJellyFishTime < 0)
	{
		MakeJellyFish();
		fJellyFishTime = RAND_RANGE_F(5.f,10.f);
	}
}

void CStageScene::MakeFish()
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCnt = rand()%2+1;

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.5f,0.4f,0.f));
		pFish->SetColor(_vec4(0.23f,0.71f,0.8f,0.8f));
		pFish->SetState(_vec3(RAND_RANGE_F(-16.f,-14.f),RAND_RANGE_F(3.f,9.f),1.f),CFish::DIR_RIGHT,CFish::TYPE_ZIGZAG,90.f);
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.5f,0.4f,0.f));
		pFish->SetColor(_vec4(0.52f,0.6f,0.89f,0.8f));
		pFish->SetState(_vec3(RAND_RANGE_F(16.f,17.f),RAND_RANGE_F(-9.f,-3.f),1.f),CFish::DIR_LEFT,CFish::TYPE_ZIGZAG,90.f);
	}

}

void CStageScene::MakeJellyFish()
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCntR = rand()%3+1;
	for (int i =0; i< RandCntR ; ++i)
	{
		RCP_JELLYFISH pJellyFish = pLayer->Add_GameObject<CJellyFish>();

		pJellyFish->SetColor(_vec4(0.26f,0.45f,0.85f,1.f));
		pJellyFish->SetState(_vec3(RAND_RANGE_F(-14.f,-10.f),RAND_RANGE_F(-12.f,-10.f), 0.f));
		pJellyFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}

	int RandCntL = rand()%3+1;
	for (int i =0; i< RandCntL ; ++i)
	{
		RCP_JELLYFISH pJellyFish = pLayer->Add_GameObject<CJellyFish>();

		pJellyFish->SetColor(_vec4(0.4f,0.6f,0.8f,1.f));
		pJellyFish->SetState(_vec3(RAND_RANGE_F(10.f,15.f),RAND_RANGE_F(-12.f,-10.f), 0.f));
		pJellyFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}
}


void CStageScene::RegenBubble()
{
	static float fBubbleTime = 1.f;
	fBubbleTime -= m_fDeltaTime;

	if (fBubbleTime < 0)
	{
		MakeBubble();
		MakeBubbleEX();
		fBubbleTime = RAND_RANGE_F(7.f,10.f);
	}
}


void CStageScene::MakeBubble()
{

	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCntR = rand()%3+1;
	for (int i =0; i< RandCntR ; ++i)
	{
		RCP_BUBBLE pBubble = pLayer->Add_GameObject<CBubble>();

		pBubble->SetColor(_vec4(0.26f,0.45f,0.85f,RAND_RANGE_F(0.3f,0.5f)));
		pBubble->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-12.f,-10.f), 0.f));
		pBubble->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}

	int RandCntL = rand()%3+1;
	for (int i =0; i< RandCntL ; ++i)
	{
		RCP_BUBBLE pBubble = pLayer->Add_GameObject<CBubble>();

		pBubble->SetColor(_vec4(0.4f,0.6f,0.8f,RAND_RANGE_F(0.2f,0.4f)));
		pBubble->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-12.f,-10.f), 0.f));
		pBubble->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}
}
void CStageScene::MakeBubbleEX()
{

	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCntL = rand()%4+3;

	for (int i =0; i< RandCntL ; ++i)
	{
		RCP_BUBBLE_EX pBubble = pLayer->Add_GameObject<CBubbleEX>();

		pBubble->SetColor(_vec4(0.4f,0.6f,0.8f,RAND_RANGE_F(0.2f,0.5f)));
		pBubble->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-12.f,-10.f), 0.f));
		pBubble->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}
}

void CStageScene::InitBubble()
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");
	int RandCnt = rand()%10+5;
	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_BUBBLE pBubble= pLayer->Add_GameObject<CBubble>();

		pBubble->SetColor(_vec4(0.23f,0.71f,0.8f,RAND_RANGE_F(0.3f,0.5f)));
		pBubble->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-12.f,10.f), 0.f));
		pBubble->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_BUBBLE_EX pBubble = pLayer->Add_GameObject<CBubbleEX>();

		pBubble->SetColor(_vec4(0.26f,0.45f,0.85f,RAND_RANGE_F(0.3f,0.6f)));
		pBubble->SetState(_vec3(RAND_RANGE_F(-15.f,15.f),RAND_RANGE_F(-12.f,10.f), 0.f));
		pBubble->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.35f,0.45f,0.f));
	}
}
