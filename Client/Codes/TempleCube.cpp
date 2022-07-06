#include "stdafx.h"
#include "TempleCube.h"
#include "CubeTemple_Texture.h"

CTempleCube::CTempleCube()
{
}

CTempleCube::~CTempleCube()
{
}

HRESULT CTempleCube::Init()
{
	m_pResource = this->AddComponent<Engine::CCubeTemple_Texture>();
	m_pResource->LoadResource<Engine::CCubeTemple_Texture>(RESOURCE_STATIC, "CUBETEMPLETEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->SetScaleY(0.5f);
	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	return S_OK;
}