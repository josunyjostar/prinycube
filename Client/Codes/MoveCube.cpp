#include "stdafx.h"
#include "MoveCube.h"

CMoveCube::CMoveCube()
: m_fTimeInterval(0)
, m_fStopInterval(0)
, m_fDeltaTime(0)
, m_fTimer(0)
, m_fStopTimer(0)
, m_bReversal(true)
{ 
}

CMoveCube::~CMoveCube()
{
}

void CMoveCube::Update()
{
	CBasicCube::Update();

	m_fDeltaTime = m_pTimer->GetFrameTime();
	m_PrevPoint = m_pTransform->GetPosition();

	m_fTimer += m_fDeltaTime/m_fTimeInterval;
	if(m_fTimer >= 1.f)
	{
		m_fTimer = 1.f;

		m_fStopTimer += m_fDeltaTime;
		if(m_fStopTimer >= m_fStopInterval)
		{
			m_fStopTimer = 0.f;
			m_fTimer = 0.f;
			m_bReversal = !m_bReversal;
		}
	}

	if(m_bReversal)
		m_pTransform->SetPosition(Lerp(m_vEndPoint, m_vStartPoint, m_fTimer));
	else
		m_pTransform->SetPosition(Lerp(m_vStartPoint, m_vEndPoint, m_fTimer));
}

void CMoveCube::Setting(RCP_STAGESCENE _StageScene, _vec3 _StartPoint, _vec3 _EndPoint, float _TimeInterval, float _StopInterval, RCP_SHADOWLIGHT _ShadowLight)
{
	m_PrevPoint = m_vStartPoint = _StartPoint;
	m_vEndPoint = _EndPoint;
	m_fTimeInterval = _TimeInterval;
	m_fStopInterval = _StopInterval;

	this->SetShadow(_StageScene, _StartPoint, _ShadowLight);
}

_vec3 CMoveCube::GetMotionVector() const
{
	return m_pTransform->GetPosition() - m_PrevPoint;
}
