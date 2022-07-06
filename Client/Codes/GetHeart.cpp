#include "stdafx.h"
#include "GetHeart.h"

CGetHeart::CGetHeart()
{
}

CGetHeart::~CGetHeart()
{
}

HRESULT CGetHeart::Init()
{
	m_bBillboard = true;

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);

	m_pTransform->SetScale(_vec3(0.5f, 0.5f, 0.5f));

	this->SetLifeTime(1.f);
	this->SetMoveDir(_vec3(0,1,0)*0.016f);

	this->Play(RESOURCE_STATIC, "TEX_GET_HEART");
	Engine::SoundMgr->Play(SND_GET_HEART);

	return S_OK;
}