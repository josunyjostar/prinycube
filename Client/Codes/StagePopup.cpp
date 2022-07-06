#include "stdafx.h"
#include "StagePopup.h"

CStagePopup::CStagePopup()
:m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_fDeltaTime(0.f)
,m_fMaxLifeTime(2.5f)
,m_fLifeTime(0.f)
,m_fUpTime(1.7f)
,m_fDownTime(2.35f)
{
}

CStagePopup::~CStagePopup()
{
}

HRESULT CStagePopup::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetZOrder(6);
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_UI);
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	m_pTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_F_STAGE_N");
	m_iTexIndex = g_GameData.RunningStage;

	m_pTransform->SetScale(UI_SIZE(448.f, 64.f));
	m_pTransform->SetPositionY(UI_Y(250.f));

	return S_OK;
}

void CStagePopup::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	if(m_fDeltaTime == 0.f) return;

	m_fLifeTime += m_fDeltaTime;
	if(m_fLifeTime >= m_fMaxLifeTime)
	{
		Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
		return;
	}

	float fVal;
	if(m_fLifeTime < m_fUpTime)
	{
		fVal = m_fLifeTime/m_fUpTime;

		m_vColor = _vec4(1,1,1,fVal*0.7f);
	}
	else if(m_fLifeTime > m_fDownTime)
	{
		fVal = (m_fLifeTime - m_fDownTime)/(m_fMaxLifeTime - m_fDownTime);

		m_vColor = _vec4(1,1,1,(1.f-fVal)*0.7f);

		m_pTransform->SetScaleY(UI_CY(64.f)*(1.f-fVal));
		m_pTransform->SetScaleX(UI_CX(448.f)/(1.f-fVal));
	}
	else
	{
		fVal = (m_fLifeTime - m_fUpTime)/(m_fDownTime - m_fUpTime);

		m_vColor = _vec4(1,1,1,0.7f);
	}

	m_pTransform->AddPositionY(m_fDeltaTime*10.f/WINCY);
}