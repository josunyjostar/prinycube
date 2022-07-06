#include "stdafx.h"
#include "ObjectTexCube.h"

CObjectTexCube::CObjectTexCube()
{
}

CObjectTexCube::~CObjectTexCube()
{
}

void CObjectTexCube::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag)
{
	m_pTexture->LoadResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

HRESULT CObjectTexCube::Init()
{
	m_pGraphicDev = Engine::CGraphicDev::Inst().GetDev();

	m_pTransform = this->AddComponent<Engine::CTransform>();
	this->AddComponent<Engine::CRenderer>();

	m_pCube = this->AddComponent<Engine::CCube_Texture>();
	m_pCube->LoadResource<Engine::CCube_Texture>(RESOURCE_STATIC, "CUBETEX");

	D3DMATERIAL9 Matrial;
	ZeroMemory(&Matrial, sizeof(D3DMATERIAL9));
	Matrial.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Matrial.Ambient	= D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	m_pCube->SetMatrial(Matrial);

	m_pTexture = this->AddComponent<Engine::CTexture>();
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	return S_OK;
}

void CObjectTexCube::Render()
{
	m_pTransform->DeviceInWorldMatrix(m_pGraphicDev);

	m_pTexture->SetTexture();

	m_pCube->Render();

	m_pGraphicDev->SetTexture(0, NULL);
}
