#include "stdafx.h"
#include "ArrowTrapCube.h"
#include "Arrow.h"

CArrowTrapCube::CArrowTrapCube()
: m_eDir(DIR_LT)
, m_fShotInterval(0)
, m_fDeltaTime(0)
, m_fTimer(0)
{

}

CArrowTrapCube::~CArrowTrapCube()
{
}

void CArrowTrapCube::Update()
{
	if(m_bHide) return;
	m_fDeltaTime = m_pTimer->GetFrameTime();

	m_fTimer += m_fDeltaTime;
	if(m_fTimer >= m_fShotInterval)
	{
		Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("Arrow")->Add_GameObject<CArrow>(true)->Setting(m_eDir, m_pTransform->GetPosition());// º¯°æ
		m_fTimer = 0;
	}
}

void CArrowTrapCube::Setting( RCP_STAGESCENE _StageScene, _vec3 _StartPoint, VIEW_DIR _Dir, float _ShotInterval, RCP_SHADOWLIGHT _ShadowLight )
{
	m_fShotInterval = _ShotInterval;
	m_eDir = _Dir;
	m_ShadowLight = _ShadowLight;
	m_pTransform->SetScale(_vec3(0.3f,0.3f,0.3f));
	this->SetShadow(_StageScene, _StartPoint, _ShadowLight);
}
