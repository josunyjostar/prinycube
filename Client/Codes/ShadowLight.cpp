#include "stdafx.h"
#include "ShadowLight.h"

CShadowLight::CShadowLight()
{
}

CShadowLight::~CShadowLight()
{
}

HRESULT CShadowLight::Init()
{
	m_pTimer = Engine::CTimerManager::Inst().FindTimer("MainApp_Frame");

	D3DXMatrixLookAtLH(&m_matLeftLight, &_vec3(50.f, 0.f, -50.f), &_vec3(0.0f, 0.0f, 0.0f), &_vec3(0.0f, 1.0f, 0.0f));
	D3DXMatrixLookAtLH(&m_matRightLight, &_vec3(-50.f, 0.f, -50.f), &_vec3(0.0f, 0.0f, 0.0f), &_vec3(0.0f, 1.0f, 0.0f));

	m_bLeft = true;
	m_fTimeVal = 0.f;

	m_matLightView = m_matLeftLight;

	return S_OK;
}

void CShadowLight::Update()
{
	float dt = m_pTimer->GetFrameTime();

	if(m_bLeft)
	{
		m_fTimeVal -= dt;
		if(m_fTimeVal < 0.f) m_fTimeVal = 0.f;
	}
	else
	{
		m_fTimeVal += dt;
		if(m_fTimeVal > 1.f) m_fTimeVal = 1.f;
	}

	m_fAccel = -cosf(m_fTimeVal*PI)*0.5f+0.5f;

	m_matLightView = Slerp(m_matLeftLight, m_matRightLight, m_fAccel);
}