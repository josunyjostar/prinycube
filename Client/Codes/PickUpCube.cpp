#include "stdafx.h"
#include "PickUpCube.h"

CPickUpCube::CPickUpCube()
:m_bThrow(false)
,m_fVelocity(5.f)
,m_fGravity(-50.f)
{
}

CPickUpCube::~CPickUpCube()
{
}

void CPickUpCube::SetHide(bool _bHide, bool _bInit /*= false*/)
{
	if(_bInit) {
		if(m_bHide && !_bHide)
		{
			m_pCollider->SetActive(!_bHide);
			m_bHide = _bHide;
		}
		else if(!m_bHide && _bHide)
		{
			m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);
			m_pCollider->SetActive(!_bHide);
		}

		m_bHide = _bHide;

		m_bStartHide = _bHide;
		m_fHideVal = (m_bHide)?0.f:1.f;
	}
	else if(m_bHide && !_bHide) {
		m_pCollider->SetActive(!_bHide);
		m_bHide = _bHide;
	}
}

HRESULT CPickUpCube::Init()
{
	m_pResource = this->AddComponent<Engine::CCube_Texture>();
	m_pResource->LoadResource<Engine::CCube_Texture>(RESOURCE_STATIC, "CUBETEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->SetScale(_vec3(0.75f, 0.75f, 0.75f));
	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	return S_OK;
}

void CPickUpCube::Update()
{
	float dt = m_pTimer->GetFrameTime();

	m_fHideVal += (m_bHide)?-dt:dt;
	if(m_fHideVal > 1.f) {
		m_fHideVal = 1.f;
		if(m_pRenderer->GetRenderType() == Engine::RENDER_ALPHA_BLEND) m_pRenderer->SetRenderType(Engine::RENDER_NORMAL);
	}
	else if(m_fHideVal < 0.f) m_fHideVal = 0.f;

	if(m_bThrow) {
		m_fThrowTime += dt;

		m_pTransform->SetPosition(m_vStartPos + m_vMove*m_fThrowTime);
		m_pTransform->SetPositionY(m_vStartPos.y + m_fGravity*SQR(m_fThrowTime)/2.f + m_fDisplacement*m_fThrowTime);
		m_pTransform->Rotate(m_pTransform->GetRightVector(), PI*4.f*dt);

		if(m_fThrowTime >= m_fEndTime) {
			m_pCollider->SetActive(true);

			m_pTransform->SetWorldMatrix(_matrix());
			m_pTransform->SetScale(_vec3(0.75f, 0.75f, 0.75f));
			m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);
			m_pTransform->SetPosition(m_vTargetPos);

			// Create Large Dust Effect

			m_pPlayer = NULL;
			m_bThrow = false;
		}
	}
	else if(m_pPlayer) {
		if(m_pPlayer->GetAnimator()->GetPlayAniName() == "TEX_PRINNY_ROTATION")
		{
			m_pTransform->Rotate(m_pTransform->GetUpVector(), dt*4.f);
		}
		else
		{
			m_eDir = m_pPlayer->GetViewDir();
			if(m_eDir != m_ePrevDir) {
				m_ePrevDir = m_eDir;
				switch(m_eDir)
				{
				case DIR_LT: m_matTarget = g_matLT; break;
				case DIR_LB: m_matTarget = g_matLB; break;
				case DIR_RB: m_matTarget = g_matRB; break;
				case DIR_RT: m_matTarget = g_matRT; break;
				}
			}

			_matrix matTarget = m_pTransform->GetScaleMatrix() * MoveMatrix(m_matTarget, m_pTransform->GetPosition());
			m_pTransform->SetWorldMatrix(Slerp(m_pTransform->GetWorldMatrix(), matTarget, 0.4f));
		}		
		m_pTransform->SetPosition(m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition() + VEC_Y(0.7f));
	}
}

void CPickUpCube::Pick(RCP_PLAYER _pPlayer)
{
	m_pPlayer = _pPlayer;
	m_ePrevDir= m_eDir = m_pPlayer->GetViewDir();
	switch(m_eDir)
	{
	case DIR_LT: m_matTarget = g_matLT; break;
	case DIR_LB: m_matTarget = g_matLB; break;
	case DIR_RB: m_matTarget = g_matRB; break;
	case DIR_RT: m_matTarget = g_matRT; break;
	}
	m_pTransform->SetWorldMatrix(m_pTransform->GetScaleMatrix() * MoveMatrix(m_matTarget, m_pTransform->GetPosition()));
	m_pCollider->SetActive(false);
}

void CPickUpCube::Throw(const _vec3& _vTargetPos)
{
	m_bThrow = true;
	m_vStartPos = m_pTransform->GetPosition();
	m_vTargetPos = _vTargetPos;

	m_fXZDist = Distance(_vec2(m_vStartPos.x, m_vStartPos.z), _vec2(m_vTargetPos.x, m_vTargetPos.z));

	m_fThrowTime = 0.f;
	m_fEndTime = m_fXZDist/m_fVelocity;

	m_vMove = _vec3(m_vTargetPos - m_vStartPos);
	m_vMove.y = 0.f;
	m_vMove = Normalize(m_vMove)*m_fVelocity;
	m_fDisplacement = (-m_fGravity*m_fXZDist)/(m_fVelocity*2.f) - (m_vStartPos.y - m_vTargetPos.y)*m_fVelocity/m_fXZDist;

	m_pTransform->LookAt(m_vMove, VEC_Y(1));
}