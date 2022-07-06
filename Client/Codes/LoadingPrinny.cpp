#include "stdafx.h"
#include "LoadingPrinny.h"

CLoadingPrinny::CLoadingPrinny()
{
}

CLoadingPrinny::~CLoadingPrinny()
{
}

HRESULT CLoadingPrinny::Init()
{
	m_pDev = Engine::CGraphicDev::Inst().GetDev();
	m_pTimer = Engine::CTimerManager::Inst().FindTimer("MainApp_Frame");

	m_pTransform = this->AddComponent<Engine::CTransform>();
	m_pTransform->SetScale(UI_SIZE(96.f, 96.f));
	m_pTransform->SetPosition(UI_POS(64.f, WINCY-64.f));

	RCP_RENDERER pRenderer = this->AddComponent<Engine::CRenderer>();
	pRenderer->SetZOrder(1);
	pRenderer->SetRenderType(Engine::RENDER_UI);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	m_pAnimator = this->AddComponent<CAnimator>();
	m_pAnimator->SetResource((RCP_RESOURCE)m_pResource);
	m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_ROTATION");

	return S_OK;
}

void CLoadingPrinny::Render()
{
	float dt = m_pTimer->GetFrameTime();

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	m_pDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetUIMatrix(m_pDev));

	m_pAnimator->Render(dt);

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CLoadingPrinny::Finish()
{
	m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WIN_LB", false);
}