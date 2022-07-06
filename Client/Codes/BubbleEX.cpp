#include "stdafx.h"
#include "BubbleEX.h"

CBubbleEX::CBubbleEX( void )
{
}

CBubbleEX::~CBubbleEX( void )
{
}

void CBubbleEX::SetState(const _vec3& pStartPos,float fLifeTime)
{	
	this->SetLifeTime(fLifeTime);
	m_pTransform->SetPosition(pStartPos); 
	this->SetMoveDir(_vec3(0.f, 0.04f ,0.f));
}

HRESULT CBubbleEX::Init()
{
	m_pRenderer->SetZOrder(3);
	m_pRenderer->SetRenderType(Engine::RENDER_PRIORITY);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);

	this->SetColor(_vec4( 1.f , 1.f , 1.f, 0.4f));
	this->SetTargetColor(_vec4( 1.f , 1.f , 1.f, 0.f));

	m_pTransform->SetScale(_vec3(RAND_RANGE_F(0.9f,1.3f), RAND_RANGE_F(0.9f,1.3f), 0.1f));
	m_pAnimator->SetPlaySpeed(RAND_RANGE_F(0.8f,1.f));

	this->Play(RESOURCE_STATIC, "TEX_BUBBLE_EX"); 

	return S_OK;
}

void CBubbleEX::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();
	if(0.f == m_fDeltaTime) return;
	
	if(m_bUseLifeTime)
	{
		m_fLifeTime -= m_fDeltaTime;
		if(m_fLifeTime <= 0.f)
		{
			Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
			return;
		}
	}

	static float fChangeDirTime = 2.f;
	fChangeDirTime -= m_fDeltaTime;

	if (fChangeDirTime < 0.f)
	{
		this->SetMoveDir(_vec3(0.f, RAND_RANGE_F(0.02f,0.04f) ,0.f));
		fChangeDirTime = RAND_RANGE_F(2.f,4.5f);
	}

	m_vMoveDir += m_vMoveDir * m_fMoveAccel;
	m_pTransform->AddPosition(m_vMoveDir*RAND_RANGE_F(0.2f,0.4f));
}

void CBubbleEX::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	CAniEffect::Render();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}
