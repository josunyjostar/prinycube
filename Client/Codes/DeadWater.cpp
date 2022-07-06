#include "stdafx.h"
#include "DeadWater.h"
#include "Player.h"
#include "Dead.h"

CDeadWater::CDeadWater( void )
{
}

CDeadWater::~CDeadWater( void )
{
}

void CDeadWater::SetCreatePos( _vec3& pPos )
{
	_matrix matWorld = m_pTransform->GetWorldMatrix();

	_matrix matView,matViewInv; 

	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matViewInv, 0, &matView);

	_vec3 vView(matViewInv._31/matViewInv._32*1.5f,1.5f,matViewInv._33/matViewInv._32*1.5f);
                
	matWorld._41 += pPos.x + vView.x;
	matWorld._42 += pPos.y + vView.y;
	matWorld._43 += pPos.z + vView.z;

	m_pTransform->SetWorldMatrix(matWorld);
}


HRESULT CDeadWater::Init()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	//m_bBillboard = true; //이거 키면 회전 안됨
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource( (RCP_RESOURCE)(m_pResource));

	this->SetLifeTime(2.f);
	this->SetColor(_vec4( 1.f , 1.f , 1.f, 0.4f));//0.3이 본게임
	this->SetTargetColor(_vec4( 1.f , 1.f , 1.f, 0.f));

	m_pTransform->SetScale(_vec3(0.5f, 0.5f, 0.1f)); //0.5가 인게임
	m_pAnimator->SetPlaySpeed(1.9f);

	this->Play(RESOURCE_STATIC, "TEX_DEADWATER",true);
	Engine::SoundMgr->Play(SND_BUBBLE);
	Engine::SoundMgr->Play(SND_DEAD_CLICK);

	return S_OK;
}
