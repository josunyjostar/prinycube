#include "stdafx.h"
#include "Splash.h"
#include "Player.h"

CSplash::CSplash( void )
{
}

CSplash::~CSplash( void )
{
}


void CSplash::SetCreatePos( _vec3& pPos )
{
	_vec3 RegenPoint = pPos;

	_matrix matWorld = m_pTransform->GetWorldMatrix();

	_matrix matView,matViewInv; // 카메라쪽으로 땡기는 공식

	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matViewInv, 0, &matView);

	_vec3 vView(matViewInv._31/matViewInv._32*1.5f,1.5f,matViewInv._33/matViewInv._32*1.5f);
	
	matWorld._41 += pPos.x + vView.x;
	matWorld._42 += pPos.y + vView.y;
	matWorld._43 += pPos.z + vView.z;

	m_pTransform->SetWorldMatrix(matWorld);
}

HRESULT CSplash::Init()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	//m_bBillboard = true; //이거 키면 회전 안됨
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	this->SetResource( (RCP_RESOURCE)(m_pResource));

	this->SetLifeTime(0.6f);
	this->SetColor(_vec4( 1.f , 1.f , 1.f, 0.48f));
	this->SetTargetColor(_vec4( 1.f , 1.f , 1.f, 0.f));

	m_pTransform->SetScale(_vec3(1.5f, 1.5f, 0.f));
	m_pAnimator->SetPlaySpeed(1.8f);

	this->Play(RESOURCE_STATIC, "TEX_SPLASH",false);

	return S_OK;
}

void CSplash::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,false);

	if(!m_pResource || !m_pTexture) return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matWorld, matView, matProjection;

	if(m_bBillboard)
		matWorld = m_pTransform->GetBillboardMatrix(Engine::CCameraManager::Inst().GetMainCam());
	else
		matWorld = m_pTransform->GetWorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProjection);

	_vec4 _vColor = m_vColor;
	m_pShader->SetVector("vColor", &_vColor);
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &matView);
	m_pShader->SetMatrix("matProj", &matProjection);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_pAnimator->GetFrame()));
	m_pShader->Begin(NULL, NULL);

	m_pShader->BeginPass(0);
	{
		if(m_bUseLifeTime)
			_vColor += (m_vTargetColor - m_vColor) * (1.f - m_fLifeTime / m_fMaxLifeTime);

		m_pAnimator->Render(m_fDeltaTime, true);
	}
	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,true);
}

