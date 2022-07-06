#include "stdafx.h"
#include "FootPrint.h"

CFootPrint::CFootPrint( void )
: m_pPlayer(NULL)
{

}

CFootPrint::~CFootPrint( void )
{

}

HRESULT CFootPrint::Init()
{
	RCP_PLAYER pPlayer= Engine::CSceneManager::Inst().GetRunningScene()->GetLayer("Player")->Find_GameObject("CPlayer");
	RCP_TRANSFORM pPlayerTrans = pPlayer->GetTransform();
	_vec3 pPos = pPlayerTrans->GetPosition() - _vec3(0.f, 0.35f, 0.f);
	float	fSwitchFoot = pPlayer->GetCurFoot();

	fSwitchFoot *= 0.05f;
	pPlayer->SwitchFoot();

	m_pResource = CEffect::AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	CEffect::SetResource(m_pResource);
	CEffect::SetTexture(RESOURCE_STATIC, "TEX_FOOTPRINT");
	CEffect::SetLifeTime((0.f, 0.8f));
	CEffect::SetColor(_vec4(1.f,1.f,1.f,0.5f));
	CEffect::SetTargetColor(_vec4(1.f,1.f,1.f,0.0f));

	Engine::SoundMgr->Play(SND_GROUND);

	m_pTransform->Rotate(_vec3(1.f,0.f,0.f), PI*0.5f);
	m_pTransform->SetScale(_vec3(0.18f, 0.18f, 0.18f));

	switch(pPlayer->GetViewDir())
	{
	case DIR_LB:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI*0.75f);
		m_pTransform->SetPosition(_vec3(pPos.x+0.05f-fSwitchFoot, pPos.y, pPos.z+0.05f+fSwitchFoot));		
		break;
	case DIR_RB:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.75f);
		m_pTransform->SetPosition(_vec3(pPos.x-0.05f+fSwitchFoot, pPos.y, pPos.z+0.05f+fSwitchFoot));
		break;
	case DIR_LT:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), -PI*0.25f);
		m_pTransform->SetPosition(_vec3(pPos.x+0.05f+fSwitchFoot, pPos.y, pPos.z-0.05f+fSwitchFoot));
		break;
	case DIR_RT:
		m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
		m_pTransform->SetPosition(_vec3(pPos.x-0.05f-fSwitchFoot, pPos.y, pPos.z-0.05f+fSwitchFoot));
		break;
	}
	return S_OK;
}
