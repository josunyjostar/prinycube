#include "stdafx.h"
#include "Dove.h"

CDove::CDove( void )
: m_fTheta(1.f)
{
}

CDove::~CDove( void )
{
}

void CDove::SetDoveState(const _vec3& pStartPos,FLYDIR eDir,FLYTYPE eType ,bool isAccel,float fCurveCycle, float fAmplitude,float fLifeTime)
{	
	m_eFlyType  = eType;
	m_eDir		= eDir;

	m_fCurveCycle = fCurveCycle;
	m_fAmplitude  = fAmplitude;
	m_fLifeTime   = fLifeTime;

	m_pTransform->SetPosition(pStartPos); //주기조절 변수 /진폭조절 변수 /라이프 타임변수 추가해야함

	if(isAccel)
		this->SetMoveAccel(RAND_RANGE_F(0.012f,0.015f));

	_vec3 vec1(0.f,0.f,0.f);
	float fTheta(0.f);
	float fReviseTheta(0.f);

	switch(m_eDir)
	{
	case DIR_LEFT:
		this->SetMoveDir(Normalize(_vec3(RAND_RANGE_F(-1.f, -0.5f), RAND_F(1.f) ,RAND_F(1.f))) * (m_fDeltaTime * RAND_F(0.3f)));
		vec1 = m_vMoveDir - pStartPos;
		fTheta =  acosf(Dot(Normalize(vec1),_vec3(1.f,0.f,0.f)));
		fReviseTheta = D2R(20.f);
		m_pTransform->Rotate(_vec3(0.f, 0.f, 1.f), fTheta);
		break;
	case DIR_RIGHT:
		this->SetMoveDir(Normalize(_vec3(RAND_RANGE_F(0.5f, 1.f), RAND_F(1.f), 0.f)) * (m_fDeltaTime * RAND_F(0.5f)));
		vec1 = m_vMoveDir - pStartPos;
		fTheta =  acosf(Dot(Normalize(vec1),_vec3(1.f,0.f,0.f)));
		fReviseTheta = D2R(55.f);
		m_pTransform->Rotate(_vec3(0.f, 0.f, 1.f), fTheta-fReviseTheta);
		break;
	}

	// m_pTransform->Rotate(_vec3(0.f, 0.f, 1.f), -fTheta);
}

HRESULT CDove::Init()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	//m_bBillboard = true;// 이거 키면 회전 안됨
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource( (RCP_RESOURCE)(m_pResource));

	this->SetLifeTime(RAND_RANGE_F(m_fLifeTime, m_fLifeTime+2.f));
	this->SetColor(_vec4( 1.f , 1.f , 1.f,0.4f));
	this->SetTargetColor(_vec4( 1.f , 1.f , 1.f, 0.f));

	m_pTransform->SetScale(_vec3(0.6f, 0.7f, 0.1f));
	m_pAnimator->SetPlaySpeed(RAND_RANGE_F(2.0f,3.2f));

	this->Play(RESOURCE_STATIC, "TEX_DOVE");

	return S_OK;
}

void CDove::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();
	if(m_fDeltaTime == 0.f) return;
	m_fTheta += (m_fCurveCycle); // fCurveCycle 주기조절  (커질수록 짧아진다)
	const static _vec3 MainCamUp = Engine::CCameraManager::Inst().GetMainCam()->GetUpVector();

	
	if(m_bUseLifeTime)
	{
		m_fLifeTime -= m_fDeltaTime;
		if(m_fLifeTime <= 0.f)
		{
			Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
			return;
		}
	}

	if(TYPE_WAVE == m_eFlyType)
	{	
		float y = MainCamUp.y * m_fAmplitude *cosf(m_fTheta) * m_fDeltaTime; // fAmplitude 진폭 조절

		if(360.f < m_fTheta)
			m_fTheta = 0;

		switch(m_eDir)
		{
		case DIR_LEFT:
			this->SetMoveDir(_vec3(-0.02f, y ,m_vMoveDir.z));
			break;
		case DIR_RIGHT:
			this->SetMoveDir(_vec3(0.025f, y ,m_vMoveDir.z));
			break;
		}
		m_pTransform->AddPosition(m_vMoveDir);
	}
	else if (TYPE_DIAGONAL==m_eFlyType)
	{
		if (DIR_LEFT == m_eDir )
			this->SetMoveDir(_vec3(-0.035f, m_vMoveDir.y ,m_vMoveDir.z));

		m_vMoveDir += m_vMoveDir * m_fMoveAccel;
		m_pTransform->AddPosition(m_vMoveDir);
	}
}