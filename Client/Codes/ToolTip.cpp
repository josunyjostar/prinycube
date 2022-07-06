#include "stdafx.h"
#include "ToolTip.h"

CToolTip::CToolTip()
:m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_fDeltaTime(0.f)
,m_fMaxLifeTime(0.f)
,m_fLifeTime(0.f)
,m_fUpTime(0.f)
,m_fDownTime(0.f)
{
}

CToolTip::~CToolTip()
{
}

void CToolTip::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	float fVal = 0.f;
	m_fLifeTime += m_fDeltaTime;
	if(m_fLifeTime < m_fUpTime)
	{
		fVal = m_fLifeTime / m_fUpTime;
	}
	else
	{
		if(m_fLifeTime > m_fDownTime)
		{
			fVal = 1.f - (m_fLifeTime - m_fDownTime) / (m_fMaxLifeTime - m_fDownTime);

			if(m_fLifeTime > m_fMaxLifeTime)
			{
				Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
				return;
			}
		}
		else
			fVal = 1.f;
	}

	m_pTransform->SetScaleY(m_vScale.y*fVal);
	m_vColor = _vec4(1,1,1,fVal*0.5f);
}

void CToolTip::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	m_pShader->SetVector("vColor", &m_vColor);
	m_pShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pShader->SetMatrix("matView", &g_matUIView);
	m_pShader->SetMatrix("matProj", &g_matUIProj);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CToolTip::PopUp(float _fLifeTime /*= 1.f*/, float _fUpTime /*= 0.2f*/, float _fDownTime /*= 0.2f*/)
{
	m_vScale = m_pTransform->GetScale();
	m_fUpTime = _fUpTime;
	m_fDownTime = _fLifeTime -  _fDownTime;
	m_fLifeTime = 0.f;
	m_fMaxLifeTime = _fLifeTime;
	m_vColor = _vec4(1,1,1,0);
}