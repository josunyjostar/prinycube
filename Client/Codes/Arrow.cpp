#include "stdafx.h"
#include "Arrow.h"
#include "Player.h"

CArrow::CArrow()
{

}

CArrow::~CArrow()
{

}

HRESULT CArrow::Init()
{
	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);
	
	this->SetLifeTime(1.f);
	this->SetMoveAccel(10.f);
	this->SetColor(_vec4(1,1,1,1));
	this->SetTargetColor(_vec4(1,1,1,0));
	this->SetTexture(RESOURCE_STATIC, "TEX_ARROW");
	this->AddComponent<Engine::CCubeCollider>()->SetSize(_vec3(0.5f,0.5f,2));

	Engine::SoundMgr->Play(SND_ARROW_FIRE);
	
	m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_ALPHATEX");
	m_pShadowCreateShader = Engine::CShaderManager::Inst().FindShader("SHADER_SHADOWCREATE");
	return S_OK;
}

void CArrow::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	if(m_bUseLifeTime)
	{
		m_fLifeTime -= m_fDeltaTime;
		if(m_fLifeTime <= 0.f)
		{
			Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
			return;
		}
	}

	m_pTransform->AddPosition(m_vMoveDir * m_fMoveAccel * m_fDeltaTime);
}

void CArrow::Render()
{
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;	

	_matrix matWorld = m_pTransform->GetWorldMatrix();

	if(m_pShadowRenderTarget[0])
	{
		m_pGraphicDev->GetRenderTarget(0, &pHWBackBuffer);
		m_pGraphicDev->GetDepthStencilSurface(&pHWDepthStencilBuffer);

		m_pShadowCreateShader->SetFloat("fAlpha", 1.f);
		m_pShadowCreateShader->SetMatrix("matWorld", &matWorld);
		m_pShadowCreateShader->SetMatrix("matLightView", &m_pShadowLight->GetLightView());
		m_pShadowCreateShader->SetMatrix("matLightProj", &m_matLightProj);
		m_pShadowCreateShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

		m_pShadowCreateShader->Begin(NULL, NULL);
		m_pShadowCreateShader->BeginPass(0);
		for(int i = 0; i < WALL_END; ++i)
		{
			m_pShadowRenderTarget[i]->GetSurfaceLevel(0, &pShadowSurface);
			m_pGraphicDev->SetRenderTarget(0, pShadowSurface);
			m_pGraphicDev->SetDepthStencilSurface(&*m_pShadowDepthStencil[i]);

			m_pResource->Render();

			pShadowSurface->Release();	
		}
		m_pShadowCreateShader->EndPass();
		m_pShadowCreateShader->End();

		m_pGraphicDev->SetRenderTarget(0, pHWBackBuffer);
		m_pGraphicDev->SetDepthStencilSurface(pHWDepthStencilBuffer);

		pHWBackBuffer->Release();
		pHWDepthStencilBuffer->Release();
	}
	CEffect::Render();
}

void CArrow::CollisionEnter( Engine::COL_INFO& _ColInfo )
{
	if(IsDynamicCast(CPlayer, _ColInfo.pObj))
	{
		Engine::SoundMgr->Play(SND_ARROW_HIT);
		((RCP_PLAYER)_ColInfo.pObj)->ArrowPushStart(m_vMoveDir * m_fMoveAccel);
	}

	this->SetActive(false);
}

void CArrow::Setting( VIEW_DIR _Dir, _vec3 _StartPoint )
{
	switch(_Dir)
	{
	case DIR_LT:
		m_pTransform->SetWorldMatrix(g_matRT);
		m_vMoveDir = VEC_LT(1);
		break;
	case DIR_LB:
		m_pTransform->SetWorldMatrix(g_matRB);
		m_pTransform->Rotate(VEC_Y(1), PI);
		m_vMoveDir = VEC_LB(1);
		break;
	case DIR_RB:
		m_pTransform->SetWorldMatrix(g_matLB);
		m_vMoveDir = VEC_RB(1);
		break;
	case DIR_RT:
		m_pTransform->SetWorldMatrix(g_matLT);
		m_pTransform->Rotate(VEC_Y(1), PI);
		m_vMoveDir = VEC_RT(1);
		break;
	}

	m_pTransform->SetScale(_vec3(1,0.5f,1));
	RCP_STAGESCENE pStageScene = Engine::CSceneManager::Inst().GetRunningScene();
	RCP_D3DTEXTURE* pRenderTarget	= pStageScene->GetShadowRenderTarget();
	RCP_SURFACE* pDepthBuffer		= pStageScene->GetShadowDepthBuffer();
	m_pShadowLight					= pStageScene->GetShadowLight();
	m_matLightProj					= pStageScene->GetLightProjMatrix();

	for(int i = 0; i < WALL_END; ++i)
	{
		m_pShadowRenderTarget[i] = pRenderTarget[i];
		m_pShadowDepthStencil[i] = pDepthBuffer[i];
	}

	m_pTransform->SetPosition(_StartPoint);
}
