#include "stdafx.h"
#include "GrayScale.h"

CGrayScale::CGrayScale()
{
}

CGrayScale::~CGrayScale()
{
}

HRESULT CGrayScale::Init()
{
	m_pRenderer->SetZOrder(10);
	m_pRenderer->SetRenderType(Engine::RENDER_UI);

	LPDIRECT3DTEXTURE9 pTexture;
	m_pGraphicDev->CreateTexture(WINCX, WINCY, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture, NULL);
	m_pRenderTarget = pTexture;
	m_pRenderTarget->Release();

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource(m_pResource);

	m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_GRAY");

	return S_OK;
}

void CGrayScale::Render()
{
	if(!m_pResource) return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pTextureSurface;

	_vec4 _vColor = m_vColor;
	_vColor += (m_vTargetColor - m_vColor) * (1.f - m_fLifeTime / m_fMaxLifeTime);

	m_pGraphicDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
	m_pRenderTarget->GetSurfaceLevel(0, &pTextureSurface);

	RECT SrcRect,DestRect;
	SetRect( &SrcRect,  0, 0, WINCX, WINCY );
	SetRect( &DestRect, 0, 0, WINCX, WINCY );

	m_pGraphicDev->StretchRect(pSurface, &SrcRect, pTextureSurface, &DestRect, D3DTEXF_NONE);

	_vec3 vScale = m_pTransform->GetScale();

	m_pShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pShader->SetMatrix("matView", &g_matUIView);
	m_pShader->SetMatrix("matProj", &g_matUIProj);
	m_pShader->SetTexture("DiffuseMap", &*m_pRenderTarget);

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();

	pSurface->Release();
	pTextureSurface->Release();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}