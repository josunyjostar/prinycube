#include "stdafx.h"
#include "Life.h"

CLife::CLife()
{
}

CLife::~CLife()
{
}

HRESULT CLife::Init()
{
	m_pRenderer->SetZOrder(6);
	m_pRenderer->SetRenderType(Engine::RENDER_UI);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);

	m_pTransform->SetScale(UI_SIZE(64.f, 64.f));

	this->Play(RESOURCE_STATIC, "TEX_LIFE");
	Engine::SoundMgr->Play(SND_GET_HEART);

	return S_OK;
}

void CLife::Render()
{
	if(!m_pResource || !m_pTexture) return;

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pShader->SetVector("vColor", &m_vColor);
	m_pShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pShader->SetMatrix("matView", &g_matUIView);
	m_pShader->SetMatrix("matProj", &g_matUIProj);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_pAnimator->GetFrame()));

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pAnimator->Render(m_fDeltaTime, true);

	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);	
}