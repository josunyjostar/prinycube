#include "stdafx.h"
#include "ToolTipTrigger.h"
#include "ToolTip.h"

CToolTipTrigger::CToolTipTrigger()
:m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTransform(AddComponent<Engine::CTransform>())
,m_wResourceIdx(0)
,m_pResourceName(NULL)
,m_fLifeTime(0.f)
,m_fUpTime(0.f)
,m_fDownTime(0.f)
{
	RCP_SPHERE pSphere = this->AddComponent<Engine::CSphereCollider>();
	pSphere->SetTrigger(true);
	pSphere->SetRadius(1.2f);
}

CToolTipTrigger::~CToolTipTrigger()
{
}

void CToolTipTrigger::SetToolTip(const _ushort& _pResIdx, const char* _pResName, float _fLifeTime, float _fUpTime, float _fDownTime, const _vec2 _vScale, const _vec2 _vOffset)
{
	m_wResourceIdx = _pResIdx;
	m_pResourceName = _pResName;
	m_fLifeTime = _fLifeTime;
	m_fUpTime = _fUpTime;
	m_fDownTime = _fDownTime;
	m_vScale = _vScale;
	m_vOffSet = _vOffset;
}

void CToolTipTrigger::TriggerEnter(Engine::COL_INFO& _ColInfo)
{
	RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("UILayer");
	RCP_TOOLTIP pUI = pLayer->Add_GameObject<CToolTip>(true);
	pUI->SetTexture(m_wResourceIdx, m_pResourceName);
	pUI->SetScale(m_vScale);
	pUI->SetPosition(_ColInfo.pObj->GetComponent<Engine::CTransform>()->GetScreenPos(m_pGraphicDev, WINCX, WINCY) + m_vOffSet);
	pUI->PopUp(m_fLifeTime, m_fUpTime, m_fDownTime);

	Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
}