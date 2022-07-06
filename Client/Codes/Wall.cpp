#include "stdafx.h"
#include "Wall.h"

CWall::CWall()
:m_fDiffuse(1.f)
,m_iTexIndex(0)
{
}

CWall::~CWall()
{
}

void CWall::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, int _TexIndex /*= 0*/)
{
	m_pTexture->LoadResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
	m_iTexIndex = _TexIndex;
}

HRESULT CWall::Init()
{
	m_pDev = Engine::CGraphicDev::Inst().GetDev();
	m_pTransform = this->AddComponent<Engine::CTransform>();
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetZOrder(4);
	m_pRenderer->SetRenderType(Engine::RENDER_PRIORITY);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	m_pTexture = this->AddComponent<Engine::CTexture>();
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_LIGHT");

	m_pTransform->SetScale(_vec3(15.f, 15.f, 1.f));
	this->AddComponent<Engine::CCubeCollider>();

	m_pShadowApplyShader = Engine::CShaderManager::Inst().FindShader("SHADER_SHADOWAPPLY");
	m_pColorShader = Engine::CShaderManager::Inst().FindShader("SHADER_DIFFUSECOLORTEX");

	return S_OK;
}

void CWall::Render()
{
	_matrix matView, matProj;
	m_pDev->GetTransform(D3DTS_VIEW, &matView);
	m_pDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	_matrix matLightView;
	if(m_pShadowLight) matLightView = m_pShadowLight->GetLightView();

	m_pShadowApplyShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pShadowApplyShader->SetMatrix("matView", &matView);
	m_pShadowApplyShader->SetMatrix("matProj", &matProj);
	m_pShadowApplyShader->SetMatrix("matLightView", &matLightView);
	m_pShadowApplyShader->SetMatrix("matLightProj", &m_matLightProj);
	m_pShadowApplyShader->SetFloat("fDiffuse", g_fLightAmount);
	m_pShadowApplyShader->SetTexture("ShadowMap", &*m_pShadowRenderTarget);
	m_pShadowApplyShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_iTexIndex));

	m_pShadowApplyShader->Begin(NULL, NULL);
	m_pShadowApplyShader->BeginPass(0);

	m_pResource->Render();

	m_pShadowApplyShader->EndPass();
	m_pShadowApplyShader->End();

	m_pShadowApplyShader->SetTexture("ShadowMap", NULL);

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if(!m_pShadowRenderTarget) return;

	LPDIRECT3DSURFACE9 pHWBackBuffer;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer;
	LPDIRECT3DSURFACE9 pShadowSurface;

	m_pDev->GetRenderTarget(0, &pHWBackBuffer);
	m_pDev->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface);
	m_pDev->SetRenderTarget(0, pShadowSurface);
	m_pDev->SetDepthStencilSurface(&*m_pShadowDepthStencil);

	m_pDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	pShadowSurface->Release();

	m_pDev->SetRenderTarget(0, pHWBackBuffer);
	m_pDev->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWDepthStencilBuffer->Release();
}