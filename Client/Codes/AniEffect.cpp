#include "stdafx.h"
#include "AniEffect.h"

CAniEffect::CAniEffect()
{
}

CAniEffect::~CAniEffect()
{
}

void CAniEffect::SetResource(RCP_RESOURCE _pResource)
{
	m_pResource = _pResource;

	if(!m_pAnimator) m_pAnimator = this->AddComponent<CAnimator>();
	m_pAnimator->SetResource(m_pResource);
}

void CAniEffect::Play(int _iContainerIdx, const _char* _pResourceTag, bool _bRepeatPlay /*= true*/)
{
	m_pTexture = m_pAnimator->Play(_iContainerIdx, _pResourceTag, _bRepeatPlay);
}

void CAniEffect::Render()
{
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

	if(m_bUseLifeTime)
		_vColor += (m_vTargetColor - m_vColor) * (1.f - m_fLifeTime / m_fMaxLifeTime);

	m_pShader->SetVector("vColor", &_vColor);
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &matView);
	m_pShader->SetMatrix("matProj", &matProjection);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_pAnimator->GetFrame()));

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pAnimator->Render(m_fDeltaTime, true);

	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}