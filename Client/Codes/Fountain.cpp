#include "stdafx.h"
#include "Fountain.h"

CFountain::CFountain( void )
{
}

CFountain::~CFountain( void )
{
}

HRESULT CFountain::Init()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	Engine::SoundMgr->Play(SND_WATER_CLICK);
	//m_bBillboard = true; //이거 키면 회전 안됨
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource( (RCP_RESOURCE)(m_pResource));

	this->SetLifeTime(0.5f);
	this->SetColor(_vec4( 1.f , 1.f , 1.f, 0.35f));
	this->SetTargetColor(_vec4( 1.f , 1.f , 1.f, 0.f));

	m_pTransform->SetScale(_vec3(0.6f, 0.6f, 0.f));
	m_pAnimator->SetPlaySpeed(1.9f);

	this->Play(RESOURCE_STATIC, "TEX_FOUNTAIN",false);

	return S_OK;
}

void CFountain::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,false);

	if(!m_pResource || !m_pTexture) return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matWorld, matView, matProjection;

	if(m_bBillboard)
		matWorld = m_pTransform->GetBillboardMatrix(Engine::CCameraManager::Inst().GetMainCam());
	else
		matWorld = m_pTransform->GetWorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProjection);

	_vec4 _vColor = m_vColor;
	m_pShader->SetVector("vColor", &_vColor);
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &matView);
	m_pShader->SetMatrix("matProj", &matProjection);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_pAnimator->GetFrame()));
	m_pShader->Begin(NULL, NULL);

	m_pShader->BeginPass(0);
	{
		if(m_bUseLifeTime)
			_vColor += (m_vTargetColor - m_vColor) * (1.f - m_fLifeTime / m_fMaxLifeTime);

		m_pAnimator->Render(m_fDeltaTime, true);
	}
	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,true);
}

