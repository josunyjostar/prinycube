#include "stdafx.h"
#include "Slider.h"

CSlider::CSlider()
:m_fMinVal(0.f)
,m_fMaxVal(1.f)
,m_fVal(Engine::SoundMgr->GetVolume())
{
}

CSlider::~CSlider()
{
	m_pSliderHandle = NULL;
}

HRESULT CSlider::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetZOrder(6);
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_UI);
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	this->SetTexture(RESOURCE_STATIC, "TEX_SLIDER");
	this->SetScale(_vec2(320.f, 16.f));
	this->SetColor(_vec4(0.5f,0.5f,0.5f,1));

	m_pSliderHandle = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer(this->GetTag())->Add_GameObject<CSliderHandle>();
	m_pSliderHandle->GetComponent<Engine::CRenderer>()->SetZOrder(12);
	m_pSliderHandle->SetTexture(RESOURCE_STATIC, "TEX_SLIDER_HANDLE");
	m_pSliderHandle->SetScale(_vec2(48.f, 24.f));
	m_pSliderHandle->SetColor(_vec4(1,1,1,1));

	return S_OK;
}

void CSlider::Update()
{
	if(m_pSliderHandle->IsClick()) {
		_vec2 vPos = m_pTransform->GetPosition();
		vPos.x = (0.5f + vPos.x) * WINCX;
		vPos.y = (0.5f - vPos.y) * WINCY;
		_vec2 vMousePos = Engine::KeyInput->MousePos();

		if(vMousePos.x < vPos.x-152.f)		m_fVal = 0.f;
		else if(vMousePos.x > vPos.x+152.f) m_fVal = 1.f;
		else m_fVal = (vMousePos.x - (vPos.x-152.f))/304.f;

		Engine::SoundMgr->SetVolume(m_fVal);
	}
}

void CSlider::Render()
{
	_vec2 vPos = m_pTransform->GetPosition();
	vPos.x = (0.5f + vPos.x) * WINCX;
	vPos.y = (0.5f - vPos.y) * WINCY;
	m_pSliderHandle->SetPosition(_vec2(vPos.x - 152.f + m_fVal * 304.f, vPos.y));

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	m_pShader->SetVector("vColor", &m_vColor);
	m_pShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pShader->SetMatrix("matView", &g_matUIView);
	m_pShader->SetMatrix("matProj", &g_matUIProj);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_iTexIndex));

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}