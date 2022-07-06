#include "stdafx.h"
#include "TitleScene.h"

#include "BTSceneChange.h"
#include "BTExit.h"
#include "BackGround.h"

#include "Fish.h"
#include "JellyFish.h"

CTitleScene::CTitleScene()
:m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
{

}

CTitleScene::~CTitleScene()
{
}

void CTitleScene::Enable()
{
	CScene::Enable();

	if(!Engine::SoundMgr->IsPlay(BGM_TITLE))
	{
		Engine::SoundMgr->AllStop();		
		Engine::SoundMgr->Play(BGM_TITLE);
	}

	static _matrix matView;
	static _matrix matProj(2,0,0,0, 0,2,0,0, 0,0,1,1, 0,0,0,1);

	RCP_CAMERA pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(matView);
	pMainCam->SetProjMatrix(matProj);
	
	Engine::CLightManager::Inst().GetLight("DirectionLight")->DeviceInLight();

	RCP_LAYER& pObjLayer = this->Create_Layer("UILayer");
	m_pFadeInOutFilter = pObjLayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->FadeOut();

	pObjLayer->Add_GameObject<CBackGround>(true)->SetTexture(RESOURCE_STATIC, "TEX_SEA_BGA1");
	//

	RCP_BTSCENECHANGE pSceneChangeButton = pObjLayer->Add_GameObject<CBTSceneChange>(true);
	pSceneChangeButton->SetScene("CStageSelectScene", false);
	pSceneChangeButton->SetTexture(RESOURCE_STATIC, "TEX_F_GAMESTART");
	pSceneChangeButton->SetScale(_vec2(320.f, 32.f));
	pSceneChangeButton->SetPosition(_vec2(WINCX/2.f, WINCY*3.f/4.f));

	RCP_BTEXIT pExitButton = pObjLayer->Add_GameObject<CBTExit>(true);
	pExitButton->SetTexture(RESOURCE_STATIC, "TEX_F_EXIT");
	pExitButton->SetScale(_vec2(128.f, 32.f));
	pExitButton->SetPosition(_vec2(WINCX/2.f, WINCY*3.5f/4.f));

	RCP_UI pUI = pObjLayer->Add_GameObject<CUI>(true);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_TITLE_NAME");
	pUI->SetScale(_vec2(1024.f, 392.f));
	pUI->SetPosition(_vec2(WINCX/2.f, WINCY/3.f));

	//처음에 몇마리 띄울려고 
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCnt = rand()%10+8;

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->SetState(_vec3(RAND_RANGE_F(-5.f,5.f),RAND_RANGE_F(-5.f,5.f), 12.f),CFish::DIR_LEFT,CFish::TYPE_ZIGZAG);
		pFish->SetColor(_vec4(0.33f,0.89f,0.86f,0.4f)); //연한 녹색
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->SetState(_vec3(RAND_RANGE_F(-5.f,5.f),RAND_RANGE_F(-5.f,5.f), 11.f),CFish::DIR_RIGHT,CFish::TYPE_ZIGZAG);
		pFish->SetColor(_vec4(0.26f,0.45f,0.85f,0.3f)); //진한 파랑
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->SetState(_vec3(RAND_RANGE_F(-9.f,-7.f),RAND_RANGE_F(2.f,5.f),11.f),CFish::DIR_RIGHT,CFish::TYPE_ZIGZAG);
		pFish->SetColor(_vec4(0.26f,0.45f,0.85f,0.3f)); //진한 파랑
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();//회색
		pFish->SetState(_vec3(RAND_RANGE_F(0.f,5.f),RAND_RANGE_F(2.f,5.f),11.f),CFish::DIR_RIGHT,CFish::TYPE_ZIGZAG);
	}

	for (int i =0; i< 20 ; ++i)
	{
		RCP_JELLYFISH pJellyFish = pLayer->Add_GameObject<CJellyFish>();
		pJellyFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.08f,0.15f,0.f));
		pJellyFish->SetState(_vec3(RAND_RANGE_F(-5.f,5.f),RAND_RANGE_F(-5.f,5.f), 8.f));
	//	pJellyFish->SetColor(_vec4(RAND_RANGE_F(0.3f,0.41f),RAND_RANGE_F(0.3f,0.42f),RAND_RANGE_F(0.7f,0.99f),RAND_RANGE_F(0.3f,0.5f))); // 보통 보라
	}
}

void CTitleScene::Disable()
{
	this->Clear();
}

void CTitleScene::Update()
{
	CScene::Update();
	m_fDeltaTime = m_pTimer->GetFrameTime();

	void* pData =NULL;
	static float fFishTime =-1.f;
	fFishTime -= m_fDeltaTime;

	if (fFishTime < 0)
	{
		MakeFish(pData);
		fFishTime = RAND_RANGE_F(4.f,8.f);
	}

	static float fJellyFishTime = -1.f;
	fJellyFishTime -= m_fDeltaTime;

	if (fJellyFishTime < 0)
	{
		MakeJellyFish(pData);
		fJellyFishTime = RAND_RANGE_F(4.f,7.f);
	}

}

void CTitleScene::MakeFish( void* pData )
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCnt = rand()%2+3;

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->SetState(_vec3(RAND_RANGE_F(-9.f,-7.f),RAND_RANGE_F(-5.f,-0.f),11.f),CFish::DIR_RIGHT,CFish::TYPE_ZIGZAG);
		pFish->SetColor(_vec4(0.26f,0.45f,0.85f,0.3f)); //진한 파랑
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();//회색
		pFish->SetState(_vec3(RAND_RANGE_F(-9.f,-7.f),RAND_RANGE_F(2.f,5.f),11.f),CFish::DIR_RIGHT,CFish::TYPE_ZIGZAG);
	}

	for (int i =0; i< RandCnt ; ++i)
	{
		RCP_FISH pFish = pLayer->Add_GameObject<CFish>();
		pFish->SetState(_vec3(RAND_RANGE_F(7.f,9.f),RAND_RANGE_F(0.f,5.f),11.f),CFish::DIR_LEFT,CFish::TYPE_ZIGZAG);
		pFish->SetColor(_vec4(0.33f,0.89f,0.86f,0.4f)); //연한 녹색
	}
}

void CTitleScene::MakeJellyFish( void* pData )
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	int RandCntR = rand()%2+3;

	for (int i =0; i< RandCntR ; ++i)
	{
		RCP_JELLYFISH pJellyFish = pLayer->Add_GameObject<CJellyFish>();
		pJellyFish->SetState(_vec3(RAND_RANGE_F(3.f,4.5f),RAND_RANGE_F(-9.f,-8.f),9.f));
		pJellyFish->SetColor(_vec4(RAND_RANGE_F(0.1f,0.23f),RAND_RANGE_F(0.5f,0.71f),RAND_RANGE_F(0.6f,0.8f),RAND_RANGE_F(0.3f,0.5f)));
		pJellyFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.08f,0.15f,0.f));
	}

	int RandCntL = rand()%2+3;

	for (int i =0; i< RandCntL ; ++i)
	{
		RCP_JELLYFISH pJellyFish = pLayer->Add_GameObject<CJellyFish>();
		pJellyFish->SetState(_vec3(RAND_RANGE_F(-3.f,-2.5f),RAND_RANGE_F(-7.f,-6.f),9.f));
		pJellyFish->SetColor(_vec4(0.46f,0.65f,0.85f,0.3f));
		pJellyFish->GetComponent<Engine::CTransform>()->SetScale(_vec3(0.08f,0.15f,0.f));
	}
}
