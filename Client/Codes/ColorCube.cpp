#include "stdafx.h"
#include "ColorCube.h"

CColorCube::CColorCube()
: m_fDeltaTime(0.f)
{
}

CColorCube::~CColorCube()
{
	m_CubeList.clear();
}

HRESULT CColorCube::Init()
{
	m_pResource = this->AddComponent<Engine::CCube_Texture>();
	m_pResource->LoadResource<Engine::CCube_Texture>(RESOURCE_STATIC, "CUBETEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	m_vColor = _vec4(0.f, 0.f, 0.f, 1.f);

	return S_OK;
}

void CColorCube::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	if(m_vState.x >= 0.9f)
	{
		m_vColor.x += m_fDeltaTime*0.25f;
		if(m_vColor.x > 1.f)
			m_vColor.x = 1.f;
	}
	else
	{
		m_vColor.x -= m_fDeltaTime*0.25f;
		if(m_vColor.x < 0.f)
			m_vColor.x = 0.f;
	}


	if(m_vState.y >= 0.9f)
	{
		m_vColor.y += m_fDeltaTime*0.25f;
		if(m_vColor.y > 1.f)
			m_vColor.y = 1.f;
	}
	else
	{
		m_vColor.y -= m_fDeltaTime*0.25f;
		if(m_vColor.y < 0.f)
			m_vColor.y = 0.f;
	}


	if(m_vState.z >= 0.9f)
	{
		m_vColor.z += m_fDeltaTime*0.25f;
		if(m_vColor.z > 1.f)
			m_vColor.z = 1.f;
	}
	else
	{
		m_vColor.z -= m_fDeltaTime*0.25f;;
		if(m_vColor.z < 0.f)
			m_vColor.z = 0.f;
	}

	m_vState = _vec3(0.f, 0.f, 0.f);
}

void CColorCube::Render()
{
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;	

	_matrix matWorld;
	if(!m_pParent) 	
		matWorld = m_pTransform->GetWorldMatrix();

	else {
		matWorld = m_matLocalWorld * m_pParent->GetInvScaleMatrix() * m_pParent->GetWorldMatrix();
		m_pTransform->SetWorldMatrix(matWorld);
	}

	if(m_pShadowRenderTarget[0])
	{
		m_pGraphicDev->GetRenderTarget(0, &pHWBackBuffer);
		m_pGraphicDev->GetDepthStencilSurface(&pHWDepthStencilBuffer);

		m_pShadowCreateShader->SetFloat("fAlpha", 0.f);
		m_pShadowCreateShader->SetMatrix("matWorld", &matWorld);		
		m_pShadowCreateShader->SetMatrix("matLightProj", &m_matLightProj);
		m_pShadowCreateShader->SetMatrix("matLightView", &m_pShadowLight->GetLightView());
		m_pShadowCreateShader->SetTexture("DiffuseMap", NULL);

		m_pShadowCreateShader->Begin(NULL, NULL);
		m_pShadowCreateShader->BeginPass(0);

		for(int i = 0; i < WALL_END; ++i)
		{
			m_pShadowRenderTarget[i]->GetSurfaceLevel(0, &pShadowSurface);
			m_pGraphicDev->SetRenderTarget(0, pShadowSurface);
			m_pGraphicDev->SetDepthStencilSurface(&*m_pShadowDepthStencil[i]);

			m_pResource->Render();

			pShadowSurface->Release();	
		}
		m_pShadowCreateShader->EndPass();
		m_pShadowCreateShader->End();

		m_pGraphicDev->SetRenderTarget(0, pHWBackBuffer);
		m_pGraphicDev->SetDepthStencilSurface(pHWDepthStencilBuffer);

		pHWBackBuffer->Release();
		pHWDepthStencilBuffer->Release();
	}

	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vLightDir = (_vec3)Engine::CLightManager::Inst().GetLight("DirectionLight")->GetLight().Direction;
	m_pColorShader->SetVector("vLightDir", &_vec4(vLightDir, 1.f));
	m_pColorShader->SetVector("vColor", &(m_vColor+_vec4(0.2f, 0.2f, 0.2f, 0.f)));
	m_pColorShader->SetMatrix("matWorld", &matWorld);
	m_pColorShader->SetMatrix("matView", &matView);
	m_pColorShader->SetMatrix("matProj", &matProj);
	m_pColorShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

	m_pColorShader->Begin(NULL, NULL);
	m_pColorShader->BeginPass(0);

	m_pResource->Render();

	m_pColorShader->EndPass();
	m_pColorShader->End();
}

void CColorCube::StageSwap(_bool bSet)
{
	LIST_CUBE::iterator Iter = m_CubeList.begin();
	LIST_CUBE::iterator EndIter = m_CubeList.end();

	while(Iter != EndIter)
	{
		(*Iter)->SetHide(bSet);
		Iter++;
	}
}

void CColorCube::StageSwap()
{
	LIST_CUBE::iterator Iter = m_CubeList.begin();
	LIST_CUBE::iterator EndIter = m_CubeList.end();

	while(Iter != EndIter)
	{
		(*Iter)->SetHide(!(*Iter)->GetHide());
		Iter++;
	}
}