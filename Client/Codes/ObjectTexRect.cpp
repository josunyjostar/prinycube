#include "stdafx.h"
#include "ObjectTexRect.h"

CObjectTexRect::CObjectTexRect()
:m_bBillboard(false)
{
}

CObjectTexRect::~CObjectTexRect()
{
}

void CObjectTexRect::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag)
{
	m_pTexture->LoadResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

HRESULT CObjectTexRect::Init()
{
	m_pGraphicDev = Engine::CGraphicDev::Inst().GetDev();

	m_pTransform = this->AddComponent<Engine::CTransform>();
	this->AddComponent<Engine::CRenderer>();

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	m_pTexture = this->AddComponent<Engine::CTexture>();
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	return S_OK;
}

void CObjectTexRect::Render()
{
	if(m_bBillboard) {
		_vec3 vScale = m_pTransform->GetScale();
		_matrix matWorld = _matrix(vScale.x,0,0,0, 0,vScale.y,0,0, 0,0,vScale.z,0, 0,0,0,1) * Engine::CCameraManager::Inst().GetMainCam()->GetWorldMatrix();
		memcpy(matWorld.m[3], &m_pTransform->GetPosition(), sizeof(_vec3));
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	}
	else m_pTransform->DeviceInWorldMatrix(m_pGraphicDev);

	m_pTexture->SetTexture();

	m_pResource->Render();

	m_pGraphicDev->SetTexture(0, NULL);
}