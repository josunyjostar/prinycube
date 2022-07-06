#include "stdafx.h"
#include "TempleLightSource.h"

CTempleLightSource::CTempleLightSource()
{
}

CTempleLightSource::~CTempleLightSource()
{
}

void CTempleLightSource::SetHide(bool _bHide, bool _bInit /*= false*/)
{
	if(m_bHide && !_bHide)
		m_pCollider->SetActive(!_bHide);
	if(!m_bHide && _bHide)
		m_pCollider->SetActive(!_bHide);

	m_bHide = _bHide;

	if(_bInit) m_fHideVal = (m_bHide)?0.f:1.f;
}

HRESULT CTempleLightSource::Init()
{
	this->DeleteComponent<Engine::CCubeCollider>();
	m_pCollider = NULL;
	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE_CIRCLE");
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);
	m_pTransform->SetScale(_vec3(0.2f, 0.2f, 0.2f));

	m_pColorShader = Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX");

	m_vColor = _vec4(1,1,0.2f,1);

	return S_OK;
}

void CTempleLightSource::Update()
{
	float dt = m_pTimer->GetFrameTime();

	m_fHideVal += (m_bHide)?-dt:dt;
	if(m_fHideVal > 1.f) m_fHideVal = 1.f;
	else if(m_fHideVal < 0.f) m_fHideVal = 0.f;
}

void CTempleLightSource::Render()
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

		m_pShadowCreateShader->SetFloat("fAlpha", m_fHideVal);
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

	m_pColorShader->SetVector("vColor", &(_vec4(1,1,1,m_fHideVal*0.2f)*m_vColor));
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