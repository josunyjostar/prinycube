#include "stdafx.h"
#include "MirrorCube.h"
#include "PrismTri_Texture.h"

CMirrorCube::CMirrorCube()
: m_pMetalShader(Engine::CShaderManager::Inst().FindShader("SHADER_METAL"))
, m_bCanPick(false)
{
}

CMirrorCube::~CMirrorCube()
{
}

HRESULT CMirrorCube::Init()
{
	m_pResource = this->AddComponent<Engine::CPrismTri_Texture>();
	m_pResource->LoadResource<Engine::CPrismTri_Texture>(RESOURCE_STATIC, "PRISMTEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_METAL");

	((RCP_CUBE)m_pCollider)->SetSize(_vec3());
	//m_pTransform->SetScale(_vec3(0.75f, 0.75f, 0.75f));
	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	D3DXMatrixIdentity(&m_matOrigin);

	m_vColor = _vec4(0.2f, 0.2f, 0.2f, 1.f);
	m_bState = false;

	return S_OK;
}

void CMirrorCube::Update()
{
	float dt = m_pTimer->GetFrameTime();
	_matrix test = m_pTransform->GetRotationMatrix();

	if(m_bThrow) {
		m_fThrowTime += dt;

		m_pTransform->SetPosition(m_vStartPos + m_vMove*m_fThrowTime);
		m_pTransform->SetPositionY(m_vStartPos.y + m_fGravity*SQR(m_fThrowTime)/2.f + m_fDisplacement*m_fThrowTime);
		m_pTransform->Rotate(m_pTransform->GetRightVector(), PI*4.f*dt);

		if(m_fThrowTime >= m_fEndTime) {
			m_pCollider->SetActive(true);

			m_pTransform->SetWorldMatrix(_matrix());
			m_pTransform->SetWorldMatrix(m_matOrigin);
			m_pTransform->SetPosition(m_vTargetPos);
			m_pTransform->AddPositionY( -(1.f - m_fYScale) * 0.5f );

			// Create Large Dust Effect

			m_pPlayer = NULL;
			m_bThrow = false;
			m_bState = false;
		}
	}
	else if(m_pPlayer) {
		if(m_pPlayer->GetAnimator()->GetPlayAniName() == "TEX_PRINNY_ROTATION")
		{
			m_pTransform->AddRotationY(dt*4.f);
		}
		else
		{
			m_eDir = m_pPlayer->GetViewDir();
			if(m_eDir != m_ePrevDir) {
				m_ePrevDir = m_eDir;
				switch(m_eDir)
				{
				case DIR_LT: m_matTarget = g_matLT; break;
				case DIR_LB: m_matTarget = g_matLB; break;
				case DIR_RB: m_matTarget = g_matRB; break;
				case DIR_RT: m_matTarget = g_matRT; break;
				}
				_matrix matRot;
				D3DXMatrixRotationAxis(&matRot, &Normalize(*(_vec3*)m_matTarget.m[0]), PI*0.5f);
				for(int i = 0; i < 3; ++i)
					D3DXVec3TransformNormal((_vec3*)m_matTarget.m[i], (_vec3*)m_matTarget.m[i], &matRot);

				matRot;
				D3DXMatrixRotationAxis(&matRot, &Normalize(*(_vec3*)m_matTarget.m[1]), -PI*0.25f);
				for(int i = 0; i < 3; ++i)
					D3DXVec3TransformNormal((_vec3*)m_matTarget.m[i], (_vec3*)m_matTarget.m[i], &matRot);
			}

			_matrix matTarget = m_pTransform->GetScaleMatrix() * MoveMatrix(m_matTarget, m_pTransform->GetPosition());
			m_pTransform->SetWorldMatrix(Slerp(m_pTransform->GetWorldMatrix(), matTarget, 0.4f));
		}		
		m_pTransform->SetPosition(m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition() + VEC_Y(0.3f));
	}
}

void CMirrorCube::Render()
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

	_vec3 vLightDir = (_vec3)Engine::CLightManager::Inst().GetLight("DirectionLight")->GetLight().Direction;
	m_pMetalShader->SetVector("vLightPos", &_vec4(0.f, 100.f, -100.f, 0.2f));
	m_pMetalShader->SetVector("vColor", &m_vColor);
	m_pMetalShader->SetVector("vEyePos", &_vec4(0.f, 20.f, -20.f, 1.f));
	m_pMetalShader->SetMatrix("matWorld", &matWorld);
	m_pMetalShader->SetMatrix("matView", &matView);
	m_pMetalShader->SetMatrix("matProj", &matProj);
	m_pMetalShader->SetTexture("ReflectMap", m_pTexture->GetTexture());

	m_pMetalShader->Begin(NULL, NULL);
	m_pMetalShader->BeginPass(0);

	m_pResource->Render();

	m_pMetalShader->EndPass();
	m_pMetalShader->End();
}

void CMirrorCube::Pick(RCP_PLAYER _pPlayer)
{
	m_pPlayer = _pPlayer;
	m_ePrevDir= m_eDir = m_pPlayer->GetViewDir();
	m_matOrigin = m_pTransform->GetScaleMatrix();
	m_fYScale = m_matOrigin._22;
	m_matOrigin *= m_pTransform->GetRotationMatrix();

	switch(m_eDir)
	{
	case DIR_LT: m_matTarget = g_matLT; break;
	case DIR_LB: m_matTarget = g_matLB; break;
	case DIR_RB: m_matTarget = g_matRB; break;
	case DIR_RT: m_matTarget = g_matRT; break;
	}
	//r 90 u 45
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &Normalize(*(_vec3*)m_matTarget.m[0]), PI*0.5f);
	for(int i = 0; i < 3; ++i)
		D3DXVec3TransformNormal((_vec3*)m_matTarget.m[i], (_vec3*)m_matTarget.m[i], &matRot);

	matRot;
	D3DXMatrixRotationAxis(&matRot, &Normalize(*(_vec3*)m_matTarget.m[1]), -PI*0.25f);
	for(int i = 0; i < 3; ++i)
		D3DXVec3TransformNormal((_vec3*)m_matTarget.m[i], (_vec3*)m_matTarget.m[i], &matRot);

	m_pTransform->SetWorldMatrix(m_pTransform->GetScaleMatrix() * MoveMatrix(m_matTarget, m_pTransform->GetPosition()));
	m_pCollider->SetActive(false);
}
