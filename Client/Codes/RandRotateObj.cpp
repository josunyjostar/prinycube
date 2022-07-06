#include "stdafx.h"
#include "RandRotateObj.h"

#include "Cylinder_Texture.h"
#include "PrismTri_Texture.h"
#include "CubeTri_Texture.h"
#include "CubeTemple_Texture.h"

HRESULT CRandRotateObj::Init() 
{
	m_pRenderer->SetRenderType(Engine::RENDER_NORMAL);

	switch(rand()%5) {
	case 0: m_pResource = this->AddComponent<Engine::CCube_Texture>();		m_pResource->LoadResource<Engine::CCube_Texture>(RESOURCE_STATIC, "CUBETEX");			break;
	case 1: m_pResource = this->AddComponent<Engine::CCylinder_Texture>();	m_pResource->LoadResource<Engine::CCylinder_Texture>(RESOURCE_STATIC, "CYLINDERTEX");	break;
	case 2: m_pResource = this->AddComponent<Engine::CPrismTri_Texture>();	m_pResource->LoadResource<Engine::CPrismTri_Texture>(RESOURCE_STATIC, "PRISMTEX");		break;
	case 3: m_pResource = this->AddComponent<Engine::CCubeTri_Texture>();	m_pResource->LoadResource<Engine::CCubeTri_Texture>(RESOURCE_STATIC, "CUBETRITEX");		break;
	case 4: m_pResource = this->AddComponent<Engine::CCubeTemple_Texture>();m_pResource->LoadResource<Engine::CCubeTemple_Texture>(RESOURCE_STATIC, "CUBETEMPLETEX"); break;
	}
	m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_DIFFUSECOLORTEX");
	m_pShader->SetVector("vLightDir", &_vec4((_vec3)Engine::CLightManager::Inst().GetLight("DirectionLight")->GetLight().Direction, 1.f));
	m_pTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->SetWorldMatrix(g_matLB);
	m_pTransform->SetScale(_vec3(0.3f, 0.3f, 0.3f));
	switch(rand()%3) {
	case 0: m_pTransform->SetPosition(_vec3(-1,0,1) + VEC_RT(10)); break;
	case 1: m_pTransform->SetPosition(VEC_RT(10)); break;
	case 2: m_pTransform->SetPosition(_vec3(1,0,-1) + VEC_RT(10)); break;
	}

	this->AddComponent<Engine::CSphereCollider>()->SetRadius(0.1f);

	_vec4 vRandColor = _vec4(RAND_RANGE_F(0.5f, 1.f),RAND_RANGE_F(0.5f, 1.f),RAND_RANGE_F(0.5f, 1.f),1.f);
	this->SetColor(vRandColor);
	this->SetTargetColor(vRandColor);
	this->SetLifeTime(10.f);
	this->SetMoveDir(_vec3(-1,0,-1)*RAND_RANGE_F(0.03f, 0.12f));
	this->SetRotate(m_pTransform->GetRightVector(), PI*4.f*0.016f);

	return S_OK;
}