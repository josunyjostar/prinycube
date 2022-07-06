#include "stdafx.h"
#include "Skybox.h"

CSkyBox::CSkyBox()
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetZOrder(3);
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_PRIORITY);

	m_pGraphicDev = Engine::CGraphicDev::Inst().GetDev();
	m_pTransform = AddComponent<Engine::CTransform>();

	m_pResource = AddComponent<Engine::CCube_CubeTexture>();
	m_pResource->LoadResource<Engine::CCube_CubeTexture>(RESOURCE_STATIC, "CUBECUBETEX");

	m_pTexture = AddComponent<Engine::CTexture>();
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_SKYBOX");

	return S_OK;
}

void CSkyBox::Render()
{
	_matrix	matView, matPrevProj, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPrevProj);
	D3DXMatrixPerspectiveFovLH(&matProj, PI*0.35f, float(WINCX) / WINCY, 0.1f, 100.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matView, 0, &matView);
	m_pTransform->SetPosition(*(_vec3*)matView.m[3]);
	m_pTransform->DeviceInWorldMatrix(m_pGraphicDev);

	m_pTexture->SetTexture(3);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pResource->Render();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPrevProj);
	m_pGraphicDev->SetTexture(0, NULL);
}
