#include "Light.h"
using namespace Engine;

CLight::CLight()
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

CLight::~CLight()
{
}

void CLight::InitLight(RCP_DEVICE& _pGraphicDev, const D3DLIGHT9* _Light)
{
	m_pGraphicDev		= _pGraphicDev;

	if(!_Light)
	{
		m_Light.Type		= D3DLIGHT_DIRECTIONAL;
		m_Light.Diffuse		= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Ambient		= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Direction	= Normalize(_vec3(0.1f, -1.f, 0.9f));
	}
	else
		m_Light = *_Light;
}

void CLight::DeviceInLight()
{
	m_pGraphicDev->SetLight(0, &m_Light);
}