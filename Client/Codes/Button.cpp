#include "stdafx.h"
#include "Button.h"

CButton::CButton()
:m_bEnter(false)
,m_bButtonDown(false)
{
}

CButton::~CButton()
{
}

void CButton::Update()
{
	if(!this->GetActive()) return;
	_vec3 vMousePos = _vec3(Engine::KeyInput->MousePos());

	vMousePos.x = (vMousePos.x / WINCX * 2.f - 1.f)*0.5f;
	vMousePos.y = (1.f - vMousePos.y / WINCY * 2.f)*0.5f;

	_vec3 vUIPos = m_pTransform->GetPosition();
	_vec3 vUISize= m_pTransform->GetScale();

	if(vMousePos.x > vUIPos.x - vUISize.x/2.f &&
		vMousePos.x < vUIPos.x + vUISize.x/2.f &&
		vMousePos.y > vUIPos.y - vUISize.y/2.f &&
		vMousePos.y < vUIPos.y + vUISize.y/2.f)
	{
		if(!m_bEnter) {
			m_bEnter = true;
			m_vPrevScale = vUISize;
			m_pTransform->SetScale(vUISize * 1.05f);

			Engine::SoundMgr->Play(SND_BUTTON_ENTER);

			this->OnMouseEnter();			
		}
	}
	else 
	{
		if(m_bEnter) {
			m_bEnter = false;
			m_bButtonDown = false;
			m_pTransform->SetScale(m_vPrevScale);

			this->OnMouseExit();
		}
	}

	if(m_bEnter && Engine::KeyInput->IsDown(VK_LBUTTON))
	{
		m_bButtonDown = true;
		m_pTransform->SetScale(vUISize * 0.95f);
		this->OnMouseButtonDown();
	}

	if(m_bEnter && m_bButtonDown && Engine::KeyInput->IsUp(VK_LBUTTON))
	{
		m_bButtonDown = false;
		m_pTransform->SetScale(vUISize * 1.05f);
		Engine::SoundMgr->Play(SND_BUTTON_CLICK);
		this->OnMouseButtonUp();
	}
}

void CButton::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	m_pShader->SetVector("vColor", &(m_vColor * ((m_bButtonDown)?_vec4(0.6f, 0.6f, 0.6f, 1):_vec4(1,1,1,1))));
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