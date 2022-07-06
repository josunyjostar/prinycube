#include "stdafx.h"
#include "NextCube.h"
#include "HeartBeat.h"

CNextCube::CNextCube()
:m_bAction(false)
{
}

CNextCube::~CNextCube()
{
}

HRESULT CNextCube::Init()
{
	CBasicCube::Init();

	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pMoon = this->AddComponent<Engine::CRect_Texture>();
	m_pMoon->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	m_pMoon->SetOffset(0.5f, -0.25f);
	m_pMoonTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_GATE");
	m_pAlphaShader =  Engine::CShaderManager::Inst().FindShader("SHADER_ALPHATEX");

	return S_OK;
}

void CNextCube::Update()
{
	if(m_bAction) return;

	if(Engine::KeyInput->IsDown(VK_NUMPAD5))
	{
		_vec3 vPlayerPos = m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition();
		_vec3 vPos = m_pTransform->GetPosition();

		if((vPos.y < vPlayerPos.y) && sqrDistance(vPos, vPlayerPos) < 1.f)
		{
			m_bAction = true;

			//g_GameData.Save();

			m_pFadeInOutFilter->FadeIn(3.f, FADE_NORMAL);
			_vec2 vPos = m_pTransform->GetScreenPos(m_pGraphicDev, WINCX, WINCY);
			m_pFadeInOutFilter->SetPosition(vPos);
			m_pFadeInOutFilter->SetColor(_vec4(0,0,0,1));
			m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));
			Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer")->Add_GameObject<CHeartBeat>(true);
			Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer")->Add_GameObject<CHeartBeat>(true);
			Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer")->Add_GameObject<CHeartBeat>(true);
			Engine::CSceneManager::Inst().StartScene(m_pNextSceneName, 1.f);
		}
	}
}

void CNextCube::Render()
{
	CBasicCube::Render();
	
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;	

	_matrix matWorld = m_pTransform->GetPositionMatrix();

	if(m_pShadowRenderTarget[0])
	{
		m_pGraphicDev->GetRenderTarget(0, &pHWBackBuffer);
		m_pGraphicDev->GetDepthStencilSurface(&pHWDepthStencilBuffer);

		m_pShadowCreateShader->SetFloat("fAlpha", m_fHideVal);
		m_pShadowCreateShader->SetMatrix("matWorld", &matWorld);		
		m_pShadowCreateShader->SetMatrix("matLightProj", &m_matLightProj);
		m_pShadowCreateShader->SetMatrix("matLightView", &m_pShadowLight->GetLightView());
		m_pShadowCreateShader->SetTexture("DiffuseMap", m_pMoonTexture->GetTexture());

		m_pShadowCreateShader->Begin(NULL, NULL);
		m_pShadowCreateShader->BeginPass(0);
		for(int i = 0; i < WALL_END; ++i)
		{
			m_pShadowRenderTarget[i]->GetSurfaceLevel(0, &pShadowSurface);
			m_pGraphicDev->SetRenderTarget(0, pShadowSurface);
			m_pGraphicDev->SetDepthStencilSurface(&*m_pShadowDepthStencil[i]);

			m_pMoon->Render();

			pShadowSurface->Release();	
		}
		m_pShadowCreateShader->EndPass();
		m_pShadowCreateShader->End();

		m_pGraphicDev->SetRenderTarget(0, pHWBackBuffer);
		m_pGraphicDev->SetDepthStencilSurface(pHWDepthStencilBuffer);

		pHWBackBuffer->Release();
		pHWDepthStencilBuffer->Release();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTexture(0, m_pMoonTexture->GetTexture());
	m_pMoon->Render();
}

void CNextCube::Setting( RCP_STAGESCENE _StageScene, _vec3 _StartPoint, RCP_PLAYER _Player, const char* _NextSceneName, RCP_FADEINOUTFILTER _FadeInOut, RCP_SHADOWLIGHT _ShadowLight )
{
	m_pPlayer = _Player;
	m_pNextSceneName = _NextSceneName;
	m_pFadeInOutFilter = _FadeInOut;
	this->SetShadow(_StageScene, _StartPoint, _ShadowLight);	
}