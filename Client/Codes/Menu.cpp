#include "stdafx.h"
#include "Menu.h"

#include "BTMenuBack.h"
#include "BTSceneChange.h"
#include "BTReset.h"
#include "Slider.h"
#include "GrayScale.h"

#include "Stage00.h"
#include "Stage01.h"
#include "Stage02.h"
#include "Stage01.h"
#include "Stage02.h"
#include "Stage03.h"
#include "Stage04.h"
#include "Stage05.h"
#include "Stage06_1.h"
#include "Stage07_1.h"
#include "Stage08.h"
#include "Stage09.h"


CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	m_pMenuButton->SetActive(true);
}

HRESULT CMenu::Init()
{
	Engine::CTimerManager::Inst().FindTimer("MainApp_Frame")->SetFrameStop(true);

	RCP_LAYER& pMenuLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("MenuLayer");

	Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("MenuLayer")->Add_GameObject<CGrayScale>(true);

	Engine::SoundMgr->Play(SND_PAUSE);

	RCP_UI pUI = pMenuLayer->Add_GameObject<CUI>(true);
	pUI->SetZOrder(10);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_BLACK_BGA");
	pUI->SetScale(_vec2((float)WINCX, (float)WINCY));
	pUI->SetColor(_vec4(1,1,1,0.6f));	

	pUI = pMenuLayer->Add_GameObject<CUI>(true);
	pUI->SetZOrder(11);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_F_MENU");
	pUI->SetScale(_vec2(240.f, 48.f));
	pUI->SetPosition(_vec2(WINCX/2.f, 100.f));

	RCP_BTSCENECHANGE pSceneChangeButton = pMenuLayer->Add_GameObject<CBTSceneChange>(true);
	pSceneChangeButton->SetZOrder(11);
	pSceneChangeButton->SetFadeColor(_vec4(1,1,1,1));
	pSceneChangeButton->SetFadeTargetColor(_vec4(1,1,1,1));
	pSceneChangeButton->SetFadeFilter(1.f, FADE_IN);
	pSceneChangeButton->SetTexture(RESOURCE_STATIC, "TEX_F_PREVSTAGE");
	pSceneChangeButton->SetScale(_vec2(320.f, 32.f));
	pSceneChangeButton->SetPosition(_vec2(WINCX/4.f, 300.f));
	if(g_GameData.RunningStage == 0) {
		pSceneChangeButton->SetDisable(true);
		pSceneChangeButton->SetColor(_vec4(0.5f, 0.5f, 0.5f, 1));
	}
	else {
		switch(g_GameData.RunningStage)
		{
		case 1: pSceneChangeButton->SetScene<CStage00>(true); break;
		case 2: pSceneChangeButton->SetScene<CStage01>(true); break;
		case 3: pSceneChangeButton->SetScene<CStage02>(true); break;
		case 4: pSceneChangeButton->SetScene<CStage03>(true); break;
		case 5: pSceneChangeButton->SetScene<CStage04>(true); break;
		case 6: pSceneChangeButton->SetScene<CStage05>(true); break;
		case 7: pSceneChangeButton->SetScene<CStage06_1>(true); break;
		case 8: pSceneChangeButton->SetScene<CStage07_1>(true); break;
		case 9: pSceneChangeButton->SetScene<CStage08>(true); break;
		}
	}

	pSceneChangeButton = pMenuLayer->Add_GameObject<CBTSceneChange>(true);
	pSceneChangeButton->SetZOrder(11);
	pSceneChangeButton->SetFadeColor(_vec4(1,1,1,1));
	pSceneChangeButton->SetFadeTargetColor(_vec4(1,1,1,1));
	pSceneChangeButton->SetFadeFilter(1.f, FADE_IN);
	pSceneChangeButton->SetTexture(RESOURCE_STATIC, "TEX_F_NEXTSTAGE");
	pSceneChangeButton->SetScale(_vec2(320.f, 32.f));
	pSceneChangeButton->SetPosition(_vec2(WINCX*3.f/4.f, 300.f));
	if(g_GameData.RunningStage == MAX_STAGE-1 || g_GameData.ClearStage < g_GameData.RunningStage) {
		pSceneChangeButton->SetDisable(true);
		pSceneChangeButton->SetColor(_vec4(0.5f, 0.5f, 0.5f, 1));
	}
	else {
		switch(g_GameData.RunningStage)
		{
		case 0: pSceneChangeButton->SetScene<CStage01>(true); break;
		case 1: pSceneChangeButton->SetScene<CStage02>(true); break;
		case 2: pSceneChangeButton->SetScene<CStage03>(true); break;
		case 3: pSceneChangeButton->SetScene<CStage04>(true); break;
		case 4: pSceneChangeButton->SetScene<CStage05>(true); break;
		case 5: pSceneChangeButton->SetScene<CStage06_1>(true); break;
		case 6: pSceneChangeButton->SetScene<CStage07_1>(true); break;
		case 7: pSceneChangeButton->SetScene<CStage08>(true); break;
		case 8: pSceneChangeButton->SetScene<CStage09>(true); break;
		}
	}

	RCP_BUTTON pButton = pMenuLayer->Add_GameObject<CBTReset>(true);
	pButton->SetZOrder(11);
	pButton->SetTexture(RESOURCE_STATIC, "TEX_F_RESET");
	pButton->SetScale(_vec2(160.f, 32.f));
	pButton->SetPosition(_vec2(WINCX/4.f, 400.f));

	pSceneChangeButton = pMenuLayer->Add_GameObject<CBTSceneChange>(true);
	pSceneChangeButton->SetZOrder(11);
	pSceneChangeButton->SetFadeFilter(0.5f);
	pSceneChangeButton->SetTexture(RESOURCE_STATIC, "TEX_F_TITLE");
	pSceneChangeButton->SetScale(_vec2(160.f, 32.f));
	pSceneChangeButton->SetPosition(_vec2(WINCX*3.f/4.f, 400.f));
	pSceneChangeButton->SetScene("CTitleScene");

	pUI = pMenuLayer->Add_GameObject<CUI>(true);
	pUI->SetZOrder(11);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_F_VOLUME");
	pUI->SetScale(_vec2(224.f, 32.f));
	pUI->SetPosition(_vec2(WINCX/4.f, 500.f));

	pUI = pMenuLayer->Add_GameObject<CSlider>(true);
	pUI->SetZOrder(11);
	pUI->SetPosition(_vec2(WINCX*3.f/4.f, 500.f));

	pUI = pMenuLayer->Add_GameObject<CBTMenuBack>(true);
	pUI->SetZOrder(11);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_F_BACK");
	pUI->SetScale(_vec2(128.f, 32.f));
	pUI->SetPosition(_vec2(WINCX/2.f, WINCY*3.5f/4.f));

	return S_OK;
}