#include "stdafx.h"
#include "Dust.h"

CDust::CDust( void )
{
	
}

CDust::~CDust( void )
{

}

HRESULT CDust::Init()
{
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	//m_pRenderer->SetZOrder(1);//맨앞에 잇는애들 priority랑 UI쪽만 해당
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	_vec3 pPos;
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->GetLayer("Player");
	if(pLayer) {
		RCP_PLAYER pPlayer= pLayer->Find_GameObject("CPlayer");
		RCP_TRANSFORM pPlayerTrans = pPlayer->GetTransform();
		pPos = pPlayerTrans->GetPosition();
	}

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	this->SetResource(m_pResource);
	m_pTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(RESOURCE_STATIC,"TEX_DUST");
	this->SetTexture(m_pTexture);
	m_pGraphicDev->SetTexture(0, m_pTexture->GetTexture(rand()%2));

	this->SetLifeTime(RAND_RANGE_F(1.f, 2.f));
	m_pTransform->AddRotationZ(D2R(rand()%25));

	this->SetMoveDir(_vec3(RAND_RANGE_F(-1, 1), RAND_F(1), RAND_RANGE_F(-1, 1))*0.004f);
	this->SetMoveAccel(-0.016f*RAND_RANGE_F(0.5f, 1.5f));
	this->SetColor(_vec4(0.75f+RAND_F(0.05f), 0.75f+RAND_F(0.05f), 0.70f+RAND_F(0.05f), 0.5f+RAND_F(0.2f)));
	this->SetTargetColor(_vec4(0.8f+RAND_F(0.05f), 0.8f+RAND_F(0.05f), 0.75f+RAND_F(0.05f), 0.f));
	this->SetScalling(_vec3(RAND_F(0.3f), RAND_F(0.3f), RAND_F(0.3f))*0.016f);
	this->SetScaleAccel(-0.016f*RAND_RANGE_F(0.5f, 1.5f));

	m_pTransform->SetPosition(_vec3(pPos.x+RAND_RANGE_F(-0.1f, 0.1f), pPos.y-0.35f, pPos.z-RAND_F(0.1f)));
	m_pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));

	return S_OK;
}