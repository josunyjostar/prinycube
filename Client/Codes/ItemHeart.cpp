#include "stdafx.h"
#include "ItemHeart.h"

CItemHeart::CItemHeart()
{
}

CItemHeart::~CItemHeart()
{
}

HRESULT CItemHeart::Init()
{
	m_bBillboard = true;

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);

	m_pTransform->SetScale(_vec3(0.5f, 0.5f, 0.5f));
	switch(rand()%3) {
	case 0: m_pTransform->SetPosition(_vec3(-1,0,1) + VEC_RT(10)); break;
	case 1: m_pTransform->SetPosition(VEC_RT(10)); break;
	case 2: m_pTransform->SetPosition(_vec3(1,0,-1) + VEC_RT(10)); break;
	}

	this->AddComponent<Engine::CSphereCollider>()->SetRadius(0.1f);

	this->SetLifeTime(10.f);
	this->SetMoveDir(_vec3(-1,0,-1)*RAND_RANGE_F(0.03f, 0.12f));

	this->Play(RESOURCE_STATIC, "TEX_ITEM_HEART");
	m_pAnimator->SetPlaySpeed(4.f);

	return S_OK;
}