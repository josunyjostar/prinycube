#include "stdafx.h"
#include "Fish.h"

CFish::CFish( void )
{
}

CFish::~CFish( void )
{
}

void CFish::SetState(const _vec3& pStartPos,SWIMDIR eDir,SWIMTYPE eType,float fLifeTime)
{	
	m_eSwimType = eType;
	m_eDir		= eDir;
	m_fLifeTime = fLifeTime;

	m_pTransform->SetPosition(pStartPos); 
	m_fDeltaTime = m_pTimer->GetFrameTime();

	_vec3 vec1(0.f,0.f,0.f);
	float fTheta(0.f);
	float fReviseTheta(0.f);

	switch(m_eDir)
	{
	case DIR_LEFT:
		this->SetMoveDir(_vec3(RAND_RANGE_F(-0.04f,-0.02f), RAND_RANGE_F(-0.005f,0.005f) , m_vMoveDir.z));
		break;
	case DIR_RIGHT:
		this->SetMoveDir(_vec3(RAND_RANGE_F(0.02f,0.04f), RAND_RANGE_F(-0.005f,0.005f) , m_vMoveDir.z));
		m_pTransform->AddRotationY(PI);
		break;
	}
}

HRESULT CFish::Init()
{
	m_pRenderer->SetZOrder(3);
	m_pRenderer->SetRenderType(Engine::RENDER_PRIORITY);
	
	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);
	this->SetTexture(RESOURCE_STATIC, "TEX_FISH");

	this->SetLifeTime(RAND_RANGE_F(m_fLifeTime, m_fLifeTime+2.f));
	this->SetColor(_vec4( 1.f , 1.f , 1.f, 0.4f));
	this->SetTargetColor(_vec4( 1.f , 1.f , 1.f, 0.f));

	m_pTransform->SetScale(_vec3(0.3f, 0.3f, 0.1f));

	return S_OK;
}

void CFish::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();
	if(m_fDeltaTime == 0.f) return;

	if(m_bUseLifeTime)
	{
		m_fLifeTime -= m_fDeltaTime;
		if(m_fLifeTime <= 0.f)
		{
			Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
			return;
		}
	}

	if (TYPE_ZIGZAG==m_eSwimType)
	{
		static float fChangeDirTime = 2.f;
		fChangeDirTime -= m_fDeltaTime;

		if (DIR_LEFT == m_eDir )
		{
			if (fChangeDirTime < 0.f)
			{
				this->SetMoveDir(_vec3(RAND_RANGE_F(-0.04f,-0.02f), RAND_RANGE_F(-0.005f,0.005f) ,m_vMoveDir.z));
				fChangeDirTime = RAND_RANGE_F(4.f,7.f);
			}
		}
		else if (DIR_RIGHT == m_eDir)
		{
			if (fChangeDirTime < 0.f)
			{
				this->SetMoveDir(_vec3(RAND_RANGE_F(0.02f,0.04f), RAND_RANGE_F(-0.005f,0.005f) ,m_vMoveDir.z));
				fChangeDirTime = RAND_RANGE_F(4.f,7.f);
			}
		}

		m_vMoveDir += m_vMoveDir * m_fMoveAccel;
		m_pTransform->AddPosition(m_vMoveDir*0.2f);
	}
}

void CFish::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	CEffect::Render();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}

