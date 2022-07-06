#include "stdafx.h"
#include "AfterImage.h"

CAfterIamge::CAfterIamge( void )
{

}

CAfterIamge::~CAfterIamge( void )
{

}

void CAfterIamge::Setting(RCP_PLAYER _pPlayer)
{
	m_bBillboard = true;
	m_pRenderer = this->AddComponent<Engine::CRenderer>();

	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);


	RCP_TRANSFORM pPlayerTrans = _pPlayer->GetTransform();
	_vec3 pPos = pPlayerTrans->GetPosition();
	RCP_ANIMATOR pAni = _pPlayer->GetAnimator();
	const char* pChar = pAni->GetPlayAniName();
	m_iTextureIndex = pAni->GetFrame();

	m_pResource = CEffect::AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	CEffect::SetResource(m_pResource);
	CEffect::SetTexture(RESOURCE_STATIC, pChar);

	CEffect::SetLifeTime(0.5f);

	CEffect::SetColor(_vec4(1.f,1.f,1.f,0.3f));
	CEffect::SetTargetColor(_vec4(1.f,1.f,1.f,0.0f));

	m_pTransform->SetScale(_vec3(1.f, 1.f, 1.f));

	switch(_pPlayer->GetViewDir())
	{
	case  DIR_LB:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI*0.75f);
		m_pTransform->SetPosition(_vec3(pPos.x, pPos.y, pPos.z));		
		break;
	case  DIR_RB:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.75f);
		m_pTransform->SetPosition(_vec3(pPos.x, pPos.y, pPos.z));
		break;
	case  DIR_LT:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI*0.25f);
		m_pTransform->SetPosition(_vec3(pPos.x, pPos.y, pPos.z));
		break;
	case  DIR_RT:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
		m_pTransform->SetPosition(_vec3(pPos.x, pPos.y, pPos.z));
		break;
	}
}
