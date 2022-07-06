#include "stdafx.h"
#include "LoadingScene.h"
#include "LoadingPrinny.h"
#include "BasicCube.h"

#include "UI.h"
#include "Dove.h"
#include "CubeTri_Texture.h"
#include "CubeTemple_Texture.h"
#include "Cylinder_Texture.h"
#include "BackGround.h"
#include "PrismTri_Texture.h"

#include "EndingScene.h"

CLoadingScene::CLoadingScene()
:m_pResourceMgr(&Engine::CResourceManager::Inst())
,m_pShaderMgr(&Engine::CShaderManager::Inst())
,m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_hThread(NULL)
,m_pLoadSceneName(NULL)
,m_isFinish(false)
,m_fDeltaTime(0.f)
,m_fDelayFinish(0.f)
,m_bLoadingEnd(false)
{
	InitializeCriticalSection(&m_Crt);
}

CLoadingScene::~CLoadingScene()
{
	DeleteCriticalSection(&m_Crt);
}

HRESULT CLoadingScene::Init()
{
	m_pGraphicDev = Engine::CGraphicDev::Inst().GetDev();

	return S_OK;
}

void CLoadingScene::Enable()
{
	CScene::Enable();

	D3DXMatrixLookAtLH(&m_matView, &_vec3(3.5f, 11.f, -20.f), &_vec3(3.5f, 1.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixOrthoLH(&m_matProj, 10.f*WINCX/WINCY, 10.f, -0.1f, 1000.f);

	RCP_CAMERA pMainCam = Engine::CCameraManager::Inst().SetMainCam("BasicCam");
	pMainCam->SetViewMatrix(m_matView);
	pMainCam->SetProjMatrix(m_matProj);

	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	RCP_OBJECTFONT pObjFont = pUILayer->Add_GameObject<CObjectFont>(true);
	pObjFont->GetComponent<Engine::CRenderer>()->SetZOrder(1);
	m_pFont = pObjFont->GetFont();
	m_pFont->SetColor(D3DCOLOR_XRGB(255, 255, 255));
	m_pFont->SetPosition(_vec2(116.f, WINCY-74.f));

	Engine::CLightManager::Inst().GetLight("DirectionLight")->DeviceInLight();
}

void CLoadingScene::Disable()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);	
	m_hThread = NULL;

	this->Clear();
}

void CLoadingScene::Update()
{
	CScene::Update();

	m_fDeltaTime = m_pTimer->GetFrameTime();

	if(m_isFinish) {
		m_fDelayFinish += m_fDeltaTime;

		if(m_fDelayFinish < 1.5f)
		{
			if(!m_bLoadingEnd) {
				m_pLoadingPrinny->Finish();
				m_bLoadingEnd = true;
			}
			m_pFont->SetString("Loading Finish  (PRESS ENTER KEY)");
		}
		else if(m_fDelayFinish < 3.f)
			m_pFont->SetString("Loading Finish");
		else m_fDelayFinish = 0.f;

		static bool bStartNextScene = false;
		if(!bStartNextScene && m_bLoadingEnd && Engine::KeyInput->IsDown(VK_RETURN))
		{
			Engine::SoundMgr->Play(SND_SCENE_CHANGE);
			bStartNextScene = true;
			m_pFadeInOutFilter->FadeIn();
			Engine::CSceneManager::Inst().StartScene(m_pLoadSceneName, 0.5f);
		}

		if(!bStartNextScene && m_bLoadingEnd && Engine::KeyInput->IsDown('E'))
		{
			Engine::SoundMgr->Play(SND_SCENE_CHANGE);
			bStartNextScene = true;
			m_pFadeInOutFilter->FadeIn();
			Engine::CSceneManager::Inst().StartScene<CEndingScene>(0.5f);
		}
	}

	if(Engine::KeyInput->IsDown('R'))
		m_pPlayer->GetComponent<Engine::CTransform>()->SetPosition(_vec3());

	Engine::CCollisionManager::Inst().LayerCollisionCheck("Player", "Tile");

	static float fDoveTime = 1.f;

	fDoveTime -= m_fDeltaTime;
	if (fDoveTime < 0)
	{
		void* pData =NULL;
		MakeDove(pData);
		fDoveTime = 1.f;
	}
}

void CLoadingScene::StartLoading(const char* _pLoadSceneName)
{
	if(m_hThread) return;

	m_pLoadSceneName = _pLoadSceneName;
	m_isFinish = false;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);
}

HRESULT CLoadingScene::LoadScene(const char* _pLoadSceneName)
{
	// Init Resource
	m_pShaderMgr->CreateShader("SHADER_ALPHATEX", "../Resources/Shader/AlphaShader.fx");
	m_pShaderMgr->CreateShader("SHADER_COLORTEX", "../Resources/Shader/ColorShader.fx");
	m_pShaderMgr->CreateShader("SHADER_SHADOWCREATE", "../Resources/Shader/CreateShadow.fx");
	m_pShaderMgr->CreateShader("SHADER_SHADOWAPPLY", "../Resources/Shader/ApplyShadow.fx");
	m_pShaderMgr->CreateShader("SHADER_OCEAN", "../Resources/Shader/OceanShader.fx");
	m_pShaderMgr->CreateShader("SHADER_DIFFUSECOLORTEX", "../Resources/Shader/DiffuseColorShader.fx");
	m_pShaderMgr->CreateShader("SHADER_FADEOUT", "../Resources/Shader/FadeOutShader.fx");
	m_pShaderMgr->CreateShader("SHADER_GRAY", "../Resources/Shader/Grayscale.fx");
	m_pShaderMgr->CreateShader("SHADER_METAL", "../Resources/Shader/MetalShader.fx");

m_pFont->SetString("Loading... 10%");

	m_pResourceMgr->CreateBuffer<Engine::CRect_Color>			(RESOURCE_STATIC, "RCCOL");
	m_pResourceMgr->CreateBuffer<Engine::CRect_Texture>			(RESOURCE_STATIC, "RCTEX");
	m_pResourceMgr->CreateBuffer<Engine::CTriangle_Color>		(RESOURCE_STATIC, "TRICOL");
	m_pResourceMgr->CreateBuffer<Engine::CCube_Color>			(RESOURCE_STATIC, "CUBECOL");
	m_pResourceMgr->CreateBuffer<Engine::CCube_Texture>			(RESOURCE_STATIC, "CUBETEX");
	m_pResourceMgr->CreateBuffer<Engine::CCube_CubeTexture>		(RESOURCE_STATIC, "CUBECUBETEX");
	m_pResourceMgr->CreateBuffer<Engine::CCubeTri_Texture>		(RESOURCE_STATIC, "CUBETRITEX");
	m_pResourceMgr->CreateBuffer<Engine::CCubeTemple_Texture>	(RESOURCE_STATIC, "CUBETEMPLETEX");
	m_pResourceMgr->CreateBuffer<Engine::CCylinder_Texture>		(RESOURCE_STATIC, "CYLINDERTEX");
	m_pResourceMgr->CreateBuffer<Engine::CPrismTri_Texture>		(RESOURCE_STATIC, "PRISMTEX");

m_pFont->SetString("Loading... 20%");

	// Default Texture
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_WHITE", Engine::TEX_NORMAL, "../Resources/Textures/White.bmp");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_BLACK", Engine::TEX_NORMAL, "../Resources/Textures/Black.bmp");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_BLACK_BGA", Engine::TEX_NORMAL, "../Resources/Textures/BlackBGA.png");

	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_WHITE_CIRCLE", Engine::TEX_NORMAL, "../Resources/Textures/WhiteCircle.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_LIGHT", Engine::TEX_NORMAL, "../Resources/Textures/Light.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_TITLE_NAME", Engine::TEX_NORMAL, "../Resources/Textures/TitleName.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SCREEN_OUT_IN", Engine::TEX_NORMAL, "../Resources/Textures/Effect/ScreenOut_In.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SCREEN_OUT_OUT", Engine::TEX_NORMAL, "../Resources/Textures/Effect/ScreenOut_Out.png");


	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SEA_BGA1", Engine::TEX_NORMAL, "../Resources/Textures/SeaBGA1.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SEA_BGA2", Engine::TEX_NORMAL, "../Resources/Textures/SeaBGA2.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SEA_BGA3", Engine::TEX_NORMAL, "../Resources/Textures/SeaBGA3.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SEA_BGA4", Engine::TEX_NORMAL, "../Resources/Textures/SeaBGA4.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SEA_BGA5", Engine::TEX_NORMAL, "../Resources/Textures/SeaBGA5.png");

	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SKY_BGA1", Engine::TEX_NORMAL, "../Resources/Textures/SkyBGA1.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SKY_BGA2", Engine::TEX_NORMAL, "../Resources/Textures/SkyBGA2.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SKY_BGA3", Engine::TEX_NORMAL, "../Resources/Textures/SkyBGA3.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SKY_BGA4", Engine::TEX_NORMAL, "../Resources/Textures/SkyBGA4.png");

	
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_TileClick_0%d.wav", FMOD_DEFAULT, SND_TILE_CLICK, 3, 1);

	// Prinny Texture
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WIN_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Win/Prinny_Win_LB_0%d.png", 6, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WIN_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Win/Prinny_Win_LT_0%d.png", 6, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WIN_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Win/Prinny_Win_RB_0%d.png", 6, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WIN_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Win/Prinny_Win_RT_0%d.png", 6, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_ROTATION", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Rotation/Prinny_Rotation_%02d.png", 16, 1);

m_pFont->SetString("Loading... 30%");

	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_STAND_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Stand/Prinny_Stand_LB_0%d.png", 7, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_STAND_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Stand/Prinny_Stand_LT_0%d.png", 7, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_STAND_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Stand/Prinny_Stand_RB_0%d.png", 7, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_STAND_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Stand/Prinny_Stand_RT_0%d.png", 7, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WALK_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Walk/Prinny_Walk_LB_0%d.png", 8, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WALK_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Walk/Prinny_Walk_LT_0%d.png", 8, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WALK_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Walk/Prinny_Walk_RB_0%d.png", 8, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_WALK_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Walk/Prinny_Walk_RT_0%d.png", 8, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Up_LB.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Up_LT.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Up_RB.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Up_RT.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Down_LB.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Down_LT.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Down_RB.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Jump/Prinny_Jump_Down_RT.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Dead/Prinny_Dead_LT.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Dead/Prinny_Dead_RT.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Dead/Prinny_Dead_LB.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/Dead/Prinny_Dead_RB.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeUp/Prinny_CubeUp_LB_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeUp/Prinny_CubeUp_LT_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeUp/Prinny_CubeUp_RB_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeUp/Prinny_CubeUp_RT_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_THROW_LB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeThrow/Prinny_CubeThrow_LB_0%d.png", 2, 5);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_THROW_LT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeThrow/Prinny_CubeThrow_LT_0%d.png", 2, 5);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_THROW_RB", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeThrow/Prinny_CubeThrow_RB_0%d.png", 2, 5);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PRINNY_THROW_RT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/CubeThrow/Prinny_CubeThrow_RT_0%d.png", 2, 5);

m_pFont->SetString("Loading... 60%");

	RCP_LAYER& pUILayer = this->Create_Layer("UILayer");
	m_pFadeInOutFilter = pUILayer->Add_GameObject<CFadeOutFilter>(true);
	m_pFadeInOutFilter->FadeOut();

	pUILayer->Add_GameObject<CBackGround>(true)->SetTexture(RESOURCE_STATIC, "TEX_SKY_BGA1");
	RCP_UI pUI = pUILayer->Add_GameObject<CUI>(true);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_TITLE_NAME");
	pUI->SetScale(_vec2(512.f, 196.f));
	pUI->SetPosition(_vec2(WINCX*1.2f/4.f, WINCY/4.f));

	this->Create_Layer("Tile")->Add_GameObject<CBasicCube>(true)->GetComponent<Engine::CTransform>()->SetPosition(Normalize(_vec3(1.f, 0.f, 1.f))*2.f + _vec3(0.f, 1.f, 0.f));
	this->Create_Layer("Tile")->Add_GameObject<CBasicCube>(true)->GetComponent<Engine::CTransform>()->SetScale(_vec3(5.f, 0.5f, 5.f));

	// Effect Texture
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_FOOTPRINT", Engine::TEX_NORMAL, "../Resources/Textures/Prinny/FootPrint.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_DUST", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Dust/Dust_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_DOVE", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Dove/Dove_0%d.png", 6, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_DEADWATER", Engine::TEX_NORMAL, "../Resources/Textures/Effect/DeadWater/DeadWater_%d.png", 16, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_FISH", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Fish/Fish.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_JELLYFISH", Engine::TEX_NORMAL, "../Resources/Textures/Effect/JellyFish/JellyFish_0%d.png", 4, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SPLASH", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Splash/Splash_0%d.png", 8, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_FOUNTAIN", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Fountain/Fountain_%d.png", 49, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_DEADWATER", Engine::TEX_NORMAL, "../Resources/Textures/Effect/DeadWater/DeadWater_%d.png", 10, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_ARROW", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Arrow/icicle_%d.png", 3, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_RAY", Engine::TEX_NORMAL, "../Resources/Textures/Effect/RayTex.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_BUBBLE", Engine::TEX_NORMAL, "../Resources/Textures/Effect/Bubble/Bubble_0%d.png", 9, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_BUBBLE_EX", Engine::TEX_NORMAL, "../Resources/Textures/Effect/BubbleEX/BubbleEX_0%d.png", 9, 1);

	m_pLoadingPrinny = pUILayer->Add_GameObject<CLoadingPrinny>(true);

	// Font Texture
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_GAMESTART", Engine::TEX_NORMAL, "../Resources/Textures/Font/GameStart.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_EXIT", Engine::TEX_NORMAL, "../Resources/Textures/Font/Exit.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_OPTION", Engine::TEX_NORMAL, "../Resources/Textures/Font/Option.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_RANK", Engine::TEX_NORMAL, "../Resources/Textures/Font/Rank.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_GAMEOVER", Engine::TEX_NORMAL, "../Resources/Textures/Font/GameOver.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TIME", Engine::TEX_NORMAL, "../Resources/Textures/Font/Time.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TIMEOVER", Engine::TEX_NORMAL, "../Resources/Textures/Font/TimeOver.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_X", Engine::TEX_NORMAL, "../Resources/Textures/Font/x.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP_ROTATE", Engine::TEX_NORMAL, "../Resources/Textures/Font/ToolTip_Rotate.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP_ACTIVE", Engine::TEX_NORMAL, "../Resources/Textures/Font/ToolTip_Active.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP_PICKUP", Engine::TEX_NORMAL, "../Resources/Textures/Font/ToolTip_PickUp.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP_MIRROR", Engine::TEX_NORMAL, "../Resources/Textures/Font/ToolTip_Mirror.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP_COLOR", Engine::TEX_NORMAL, "../Resources/Textures/Font/ToolTip_Color.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP", Engine::TEX_NORMAL, "../Resources/Textures/Font/ToolTip.png");

m_pFont->SetString("Loading... 70%");

	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_NUMBER", Engine::TEX_NORMAL, "../Resources/Textures/Font/Number_%d.png", 10);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_STAGE", Engine::TEX_NORMAL, "../Resources/Textures/Font/Stage.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_STAGE_N", Engine::TEX_NORMAL, "../Resources/Textures/Font/Stage_%d.png", 10);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_MENU", Engine::TEX_NORMAL, "../Resources/Textures/Font/Menu.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_SELECT", Engine::TEX_NORMAL, "../Resources/Textures/Font/Select.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_BACK", Engine::TEX_NORMAL, "../Resources/Textures/Font/Back.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_RESET", Engine::TEX_NORMAL, "../Resources/Textures/Font/Reset.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_TITLE", Engine::TEX_NORMAL, "../Resources/Textures/Font/Title.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_NEXTSTAGE", Engine::TEX_NORMAL, "../Resources/Textures/Font/NextStage.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_PREVSTAGE", Engine::TEX_NORMAL, "../Resources/Textures/Font/PrevStage.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_VOLUME", Engine::TEX_NORMAL, "../Resources/Textures/Font/Volume.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_F_CREATORS", Engine::TEX_NORMAL, "../Resources/Textures/Font/Creators.png");

m_pFont->SetString("Loading... 80%");

	pUI = pUILayer->Add_GameObject<CUI>(true);
	pUI->SetTexture(RESOURCE_STATIC, "TEX_F_TOOLTIP");
	pUI->SetScale(_vec2(512.f, 512.f));
	pUI->SetPosition(_vec2(WINCX*3.f/4.f, WINCY/2.f));

	// Object Texture
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_WALL", Engine::TEX_NORMAL, "../Resources/Textures/Tile/Wall_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_TILE", Engine::TEX_NORMAL, "../Resources/Textures/Tile/Tile_01.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_NIGHT", Engine::TEX_NORMAL, "../Resources/Textures/Night.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_DAY", Engine::TEX_NORMAL, "../Resources/Textures/Day.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_GATE", Engine::TEX_NORMAL, "../Resources/Textures/Tile/Gate.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SLIDER", Engine::TEX_NORMAL, "../Resources/Textures/UI/Slider.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_SLIDER_HANDLE", Engine::TEX_NORMAL, "../Resources/Textures/UI/SliderHandle.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_PAUSE", Engine::TEX_NORMAL, "../Resources/Textures/UI/Pause.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_METAL", Engine::TEX_NORMAL, "../Resources/Textures/metal.png");
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_ITEM_HEART", Engine::TEX_NORMAL, "../Resources/Textures/UI/Heart_0%d.png", 2, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_GET_HEART", Engine::TEX_NORMAL, "../Resources/Textures/UI/GetHeart_0%d.png", 4, 1);
	m_pResourceMgr->CreateTexture(RESOURCE_STATIC, "TEX_LIFE", Engine::TEX_NORMAL, "../Resources/Textures/UI/Life_%02d.png", 10, 1);

	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Bubble.wav", FMOD_DEFAULT, SND_BUBBLE);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_ButtonClick.wav", FMOD_DEFAULT, SND_BUTTON_CLICK);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_ButtonEnter.wav", FMOD_DEFAULT, SND_BUTTON_ENTER);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Clear.wav", FMOD_DEFAULT, SND_CLEAR);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Dead.wav", FMOD_DEFAULT, SND_DEAD);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_DeadClick_0%d.wav", FMOD_DEFAULT, SND_DEAD_CLICK, 4, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Dive_0%d.wav", FMOD_DEFAULT, SND_DIVE, 4, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Ground_0%d.wav", FMOD_DEFAULT, SND_GROUND, 3, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Jump_Down.wav", FMOD_DEFAULT, SND_JUMP_DOWN);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Jump_Up.wav", FMOD_DEFAULT, SND_JUMP_UP);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Pause.wav", FMOD_DEFAULT, SND_PAUSE);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Rotation_Active.wav", FMOD_DEFAULT, SND_ROTATION_ACTIVE);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Rotation_Swing.wav", FMOD_DEFAULT, SND_ROTATION_SWING);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Swap.wav", FMOD_DEFAULT, SND_SWAP);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Throw_0%d.wav", FMOD_DEFAULT, SND_THROW, 2, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_WaterClick_%02d.wav", FMOD_LOOP_OFF, SND_WATER_CLICK, 11, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_SceneChange.wav", FMOD_DEFAULT, SND_SCENE_CHANGE);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_ArrowFire_0%d.wav", FMOD_DEFAULT, SND_ARROW_FIRE, 3, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_ArrowHit_0%d.wav", FMOD_DEFAULT, SND_ARROW_HIT, 3, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_StoneHit_0%d.wav", FMOD_DEFAULT, SND_STONE_HIT, 3, 1);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_GetHeart.wav", FMOD_DEFAULT, SND_GET_HEART);

	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Laser.wav", FMOD_LOOP_NORMAL, SND_LASER);
	Engine::SoundMgr->AddSound("../Resources/Sound/SND_Wheel.wav", FMOD_LOOP_NORMAL, SND_WHEEL);
	Engine::SoundMgr->AddSound("../Resources/Sound/BGM/Game.mp3", FMOD_LOOP_NORMAL, BGM_GAME);
	Engine::SoundMgr->AddSound("../Resources/Sound/BGM/Stage_06_1.mp3", FMOD_LOOP_NORMAL, BGM_STAGE_06);
	Engine::SoundMgr->AddSound("../Resources/Sound/BGM/Title.mp3", FMOD_LOOP_NORMAL, BGM_TITLE);
	Engine::SoundMgr->AddSound("../Resources/Sound/BGM/Ending.mp3", FMOD_LOOP_NORMAL, BGM_ENDING);

	Engine::SoundMgr->Play(BGM_TITLE);

	m_pPlayer = this->Create_Layer("Player")->Add_GameObject<CPlayer>(true);

	m_isFinish = true;
	m_bLoadingEnd = false;
	m_fDelayFinish = 0.f;

	return S_OK;
}

unsigned int APIENTRY CLoadingScene::Thread_Main(void* _pArg)
{
	CLoadingScene* pThis = (CLoadingScene*)_pArg;
	CRITICAL_SECTION* pCrt = pThis->GetCriticalSection();

	_uint iFailedFlag = 0;

	EnterCriticalSection(pCrt);

	pThis->GetFont()->SetString("Loading... 0%");

	if(FAILED(pThis->LoadScene(pThis->GetLoadSceneName()))) iFailedFlag = 1;

	LeaveCriticalSection(pCrt);

	return iFailedFlag;
}

void CLoadingScene::MakeDove( void* pData )
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");
	pLayer->Add_GameObject<CDove>()->SetDoveState(_vec3(RAND_RANGE_F(14.3f,15.5f),RAND_RANGE_F(1.f,2.f),0.f),CDove::DIR_LEFT,CDove::TYPE_DIAGONAL);
	pLayer->Add_GameObject<CDove>()->SetDoveState(_vec3(RAND_RANGE_F(14.9f,16.f),RAND_RANGE_F(3.f,4.f),0.f),CDove::DIR_LEFT,CDove::TYPE_DIAGONAL);
	pLayer->Add_GameObject<CDove>()->SetDoveState(_vec3(RAND_RANGE_F(15.f,16.5f),RAND_RANGE_F(2.5f,3.f),0.f),CDove::DIR_LEFT,CDove::TYPE_DIAGONAL);
}
