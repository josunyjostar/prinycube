#include "stdafx.h"
#include "UI.h"

CUI::CUI()
:m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTransform(AddComponent<Engine::CTransform>())
,m_pResource(AddComponent<Engine::CRect_Texture>())
,m_pShader(Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX"))
,m_pTexture(Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE"))
,m_vColor(1,1,1,1)
,m_iTexIndex(0)
{
}

CUI::~CUI()
{
}

void CUI::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag)
{
	m_pTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

void CUI::SetPosition(_vec3 _vUIPos)
{
	m_pTransform->SetPosition(UI_POS(_vUIPos.x, _vUIPos.y));
}

void CUI::SetPosition(_vec2 _vUIPos)
{
	m_pTransform->SetPosition(UI_POS(_vUIPos.x, _vUIPos.y));
}

void CUI::SetScale(_vec3 _vUIScale)
{
	m_pTransform->SetScale(UI_SIZE(_vUIScale.x, _vUIScale.y));
}

void CUI::SetScale(_vec2 _vUIScale)
{
	m_pTransform->SetScale(UI_SIZE(_vUIScale.x, _vUIScale.y));
}

HRESULT CUI::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetZOrder(6);
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_UI);
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	return S_OK;
}

void CUI::Render()
{
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