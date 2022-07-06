#include "stdafx.h"
#include "StageSelectScene.h"

#include "BTSceneChange.h"
#include "BTExit.h"
#include "BackGround.h"

#include "Stage00.h"
#include "Stage01.h"
#include "Stage02.h"
#include "Stage03.h"
#include "Stage04.h"
#include "Stage05.h"
#include "Stage06_1.h"
#include "Stage07_1.h"
#include "Stage08.h"
#include "Stage09.h"


CStageSelectScene::CStageSelectScene()
{
}

CStageSelectScene::~CStageSelectScene()
{
}

void CStageSelectScene::Enable()
{
	CScene::Enable();

	static _matrix matView;
	static _matrix matProj(2,0,0,0, 0,2,0,0, 0,0,1,1, 0,0,0,1);

	RCP_CAMERA pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(matView);
	pMainCam->SetProjMatrix(matProj);

	Engine::CLightManager::Inst().GetLight("DirectionLight")->DeviceInLight();

	RCP_LAYER& pObjLayer = this->Create_Layer("UILayer");
	m_pFadeInOutFilter = pObjLayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->FadeOut();

	pObjLayer->Add_GameObject<CBackGround>(true)->SetTexture(RESOURCE_STATIC, "TEX_BLACK_BGA");

	RCP_UI pUI = pObjLayer->Add_GameObject<CUI>(true);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_F_SELECT");
	pUI->SetScale(_vec2(288, 48.f));
	pUI->SetPosition(_vec2(WINCX/2.f, 100.f));

	RCP_BTSCENECHANGE pSceneChangeButton;

	int EnableStage = g_GameData.ClearStage + 1;
	for(int i = 0; i < MAX_STAGE; ++i)
	{
		pSceneChangeButton = pObjLayer->Add_GameObject<CBTSceneChange>(true);
		pSceneChangeButton->SetFadeColor(_vec4(1,1,1,1));
		pSceneChangeButton->SetFadeTargetColor(_vec4(1,1,1,1));
		pSceneChangeButton->SetFadeFilter(1.f, FADE_IN);

		switch(i)
		{
		case 0: pSceneChangeButton->SetScene<CStage00>();	break;
		case 1: pSceneChangeButton->SetScene<CStage01>();	break;
		case 2: pSceneChangeButton->SetScene<CStage02>();	break;
		case 3: pSceneChangeButton->SetScene<CStage03>();	break;
		case 4: pSceneChangeButton->SetScene<CStage04>();	break;
		case 5: pSceneChangeButton->SetScene<CStage05>();	break;
		case 6: pSceneChangeButton->SetScene<CStage06_1>();	break;
		case 7: pSceneChangeButton->SetScene<CStage07_1>();	break;
		case 8: pSceneChangeButton->SetScene<CStage08>();	break;
		case 9: pSceneChangeButton->SetScene<CStage09>();	break;
		}

		pSceneChangeButton->SetTexture(RESOURCE_STATIC, "TEX_F_STAGE_N");
		pSceneChangeButton->SetTexIndex(i);
		pSceneChangeButton->SetScale(_vec2(224.f, 32.f));
		pSceneChangeButton->SetPosition(_vec2(WINCX*((i%2)?3.f:1.f)/4.f-90.f, 220.f + 80.f*(i/2)));

		pUI = pObjLayer->Add_GameObject<CUI>(true);
		pUI->SetTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_RT");
		pUI->SetScale(_vec2(64.f, 64.f));
		pUI->SetPosition(_vec2(WINCX*((i%2)?3.f:1.f)/4.f + 80.f, 220.f + 80.f*(i/2)));

		pUI = pObjLayer->Add_GameObject<CUI>(true);
		pUI->SetTexture(RESOURCE_STATIC, "TEX_F_X");
		pUI->SetScale(_vec2(16.f, 16.f));
		pUI->SetPosition(_vec2(WINCX*((i%2)?3.f:1.f)/4.f + 127.f, 225.f + 80.f*(i/2)));

		RCP_UI pUINumber1 = pObjLayer->Add_GameObject<CUI>(true);
		pUINumber1->SetTexture(RESOURCE_STATIC, "TEX_F_NUMBER");
		pUINumber1->SetScale(_vec2(32.f, 32.f));
		pUINumber1->SetPosition(_vec2(WINCX*((i%2)?3.f:1.f)/4.f + 160.f, 220.f + 80.f*(i/2)));

		RCP_UI pUINumber2 = pObjLayer->Add_GameObject<CUI>(true);
		pUINumber2->SetTexture(RESOURCE_STATIC, "TEX_F_NUMBER");
		pUINumber2->SetScale(_vec2(32.f, 32.f));
		pUINumber2->SetPosition(_vec2(WINCX*((i%2)?3.f:1.f)/4.f + 195.f, 220.f + 80.f*(i/2)));

		if(EnableStage > i)
		{
			pUINumber1->SetTexIndex(int(g_GameData.MinDeadCount[i]/10));
			pUINumber2->SetTexIndex(int(g_GameData.MinDeadCount[i]%10));
			pSceneChangeButton->SetColor(_vec4(0.8f, 0.8f, 1, 1));
		}
		else
		{
			if(EnableStage < i)
			{
				pSceneChangeButton->SetDisable(true);
				pSceneChangeButton->SetColor(_vec4(0.5f, 0.5f, 0.5f, 1));
			}
		}
	}

	pSceneChangeButton = pObjLayer->Add_GameObject<CBTSceneChange>(true);
	pSceneChangeButton->SetScene("CTitleScene", false);
	pSceneChangeButton->SetTexture(RESOURCE_STATIC, "TEX_F_BACK");
	pSceneChangeButton->SetScale(_vec2(128.f, 32.f));
	pSceneChangeButton->SetPosition(_vec2(WINCX/2.f, WINCY - 90.f));
}

void CStageSelectScene::Disable()
{
	this->Clear();
}