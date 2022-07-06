#include "stdafx.h"
#include "FadeInOutFilter.h"

CFadeOutFilter::CFadeOutFilter()
:m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTransform(AddComponent<Engine::CTransform>())
,m_pResource(AddComponent<Engine::CRect_Texture>())
,m_pTexture(AddComponent<Engine::CTexture>())
,m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_bFadeIn(false)
,m_fFadeTime(0.f)
,m_fMaxFadeTime(0.f)
,m_vColor(0,0,0,1)
,m_vTargetColor(0,0,0,1)
,m_eFadeType(FADE_NORMAL)
{
}

CFadeOutFilter::~CFadeOutFilter()
{
}

HRESULT CFadeOutFilter::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetZOrder(9);
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_UI);

	m_pResource->LoadResource<Engine::CCube_CubeTexture>(RESOURCE_STATIC, "RCTEX");

	return S_OK;
}

void CFadeOutFilter::Render()
{
	if(!m_pShader) return;

	float dt = m_pTimer->GetFrameTime();
	m_fFadeTime -= dt;
	if(m_fFadeTime < 0.f) m_fFadeTime = 0.f;

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	_vec4 vColor = m_vColor;
	vColor += (m_vTargetColor - m_vColor) * (1.f-m_fFadeTime/m_fMaxFadeTime);

	switch(m_eFadeType)
	{
	case FADE_NORMAL:
		vColor = _vec4(vColor.x,vColor.y,vColor.z,((m_bFadeIn)?1.f-m_fFadeTime/m_fMaxFadeTime:m_fFadeTime/m_fMaxFadeTime));
		break;
	case FADE_IN: {
		float fYScale = ((m_bFadeIn)?1.f-m_fFadeTime/m_fMaxFadeTime:m_fFadeTime/m_fMaxFadeTime)*7.5f;
		m_pTransform->SetScaleX(fYScale * WINCY / WINCX);
		m_pTransform->SetScaleY(fYScale);
		break;
	}
	case FADE_OUT:
		m_pShader->SetFloat("fFadeVal", ((m_bFadeIn)?m_fFadeTime/m_fMaxFadeTime:1.f-m_fFadeTime/m_fMaxFadeTime));
		break;
	}

	m_pShader->SetVector("vColor", &vColor);
	m_pShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pShader->SetMatrix("matView", &g_matUIView);
	m_pShader->SetMatrix("matProj", &g_matUIProj);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CFadeOutFilter::FadeIn(float _fFadeTime /*= 0.5f*/, FADE_FILTER_TYPE _eFadeType /*= FADE_NORMAL*/)
{
	m_bFadeIn = true;
	this->SetFadeFilter(_fFadeTime, _eFadeType);
}

void CFadeOutFilter::FadeOut(float _fFadeTime /*= 0.5f*/, FADE_FILTER_TYPE _eFadeType /*= FADE_NORMAL*/)
{
	m_bFadeIn = false;
	this->SetFadeFilter(_fFadeTime, _eFadeType);
}

void CFadeOutFilter::SetFadeFilter(float _fFadeTime, FADE_FILTER_TYPE _eFadeType)
{
	m_fFadeTime = m_fMaxFadeTime = _fFadeTime;
	m_eFadeType = _eFadeType;

	switch(m_eFadeType)
	{
	case FADE_NORMAL:
		m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");
		m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX");
		break;
	case FADE_IN:
		m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_SCREEN_OUT_IN");
		m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX");
		break;
	case FADE_OUT:
		m_pTransform->SetScaleX(7.5f * WINCY / WINCX);
		m_pTransform->SetScaleY(7.5f);
		m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_SCREEN_OUT_OUT");
		m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_FADEOUT");
		break;
	}
}