#include "stdafx.h"
#include "BackGround.h"

CBackGround::CBackGround()
:m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pResource(AddComponent<Engine::CRect_Texture>())
,m_pShader(Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX"))
,m_pTexture(Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_BLACK"))
,m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_bSwitch(false)
{
}

CBackGround::~CBackGround()
{
}

void CBackGround::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag)
{
	m_pTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

void CBackGround::SwitchTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, float _fSwitchTime /*= 1.f*/)
{
	m_bSwitch = true;
	m_fSwitchTime = m_fMaxSwitchTime = _fSwitchTime;
	m_pTargetTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

HRESULT CBackGround::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_PRIORITY);
	
	m_pResource->LoadResource<Engine::CCube_CubeTexture>(RESOURCE_STATIC, "RCTEX");

	return S_OK;
}

void CBackGround::Render()
{
	_matrix matWorld;
	m_pShader->SetVector("vColor", &_vec4(1,1,1,1));
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &g_matUIView);
	m_pShader->SetMatrix("matProj", &g_matUIProj);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();


	if(m_bSwitch) {
		float dt = m_pTimer->GetFrameTime();

		m_fSwitchTime -= dt;

		if(m_fSwitchTime < 0.f) {
			m_fSwitchTime = 0.f;
			m_bSwitch = false;
			m_pTexture = m_pTargetTexture;
		}

		_vec4 vColor = _vec4(1,1,1,1.f-m_fSwitchTime/m_fMaxSwitchTime);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		m_pShader->SetVector("vColor", &vColor);
		m_pShader->SetMatrix("matWorld", &matWorld);
		m_pShader->SetMatrix("matView", &g_matUIView);
		m_pShader->SetMatrix("matProj", &g_matUIProj);
		m_pShader->SetTexture("DiffuseMap", m_pTargetTexture->GetTexture());

		m_pShader->Begin(NULL, NULL);
		m_pShader->BeginPass(0);

		m_pResource->Render();

		m_pShader->EndPass();
		m_pShader->End();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}