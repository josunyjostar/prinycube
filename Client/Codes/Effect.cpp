#include "stdafx.h"
#include "Effect.h"

CEffect::CEffect()
:m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTransform(AddComponent<Engine::CTransform>())
,m_pRenderer(AddComponent<Engine::CRenderer>())
,m_pShader(Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX"))
,m_bBillboard(false)
,m_fDeltaTime(0.f)
,m_bUseLifeTime(false)
,m_fMaxLifeTime(0.f)
,m_fLifeTime(0.f)
,m_vColor(1.f, 1.f, 1.f, 1.f)
,m_vTargetColor(1.f, 1.f, 1.f, 1.f)
,m_fMoveAccel(0.f)
,m_fScaleAccel(0.f)
,m_fRotateSpeed(0.f)
,m_fRotateAccel(0.f)
,m_iTextureIndex(0)
{
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);
}

CEffect::~CEffect()
{
}

void CEffect::SetRotate(_vec3 _vAxis, _float _fRotateSpeed)
{
	m_vAxis = _vAxis;
	m_fRotateSpeed = _fRotateSpeed;
}

void CEffect::SetTexture(int _iContainerIdx, const _char* _pResourceTag)
{
	m_pTexture = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(_iContainerIdx, _pResourceTag);
}

void CEffect::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();

	if(m_fDeltaTime == 0.f) return;

	if(m_bUseLifeTime)
	{
		m_fLifeTime -= m_fDeltaTime;
		if(m_fLifeTime <= 0.f)
		{
			Engine::CSceneManager::Inst().GetRunningScene()->Remove_Object(this->GetTag(), this);
			return;
		}
	}

	m_vMoveDir += m_vMoveDir * m_fMoveAccel;
	m_vScalling += m_vScalling * m_fScaleAccel;
	m_fRotateSpeed += m_fRotateSpeed * m_fRotateAccel;

	m_pTransform->AddPosition(m_vMoveDir);
	m_pTransform->AddScale(m_vScalling);
	m_pTransform->Rotate(m_vAxis, m_fRotateSpeed);
}

void CEffect::Render()
{
	if(!m_pResource) return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matWorld, matView, matProj;

	if(m_bBillboard)
		matWorld = m_pTransform->GetBillboardMatrix(Engine::CCameraManager::Inst().GetMainCam());
	else
		matWorld = m_pTransform->GetWorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec4 _vColor = m_vColor;

	if(m_bUseLifeTime)
		_vColor += (m_vTargetColor - m_vColor) * (1.f - m_fLifeTime / m_fMaxLifeTime);

	m_pShader->SetVector("vColor", &_vColor);
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &matView);
	m_pShader->SetMatrix("matProj", &matProj);
	if(m_pTexture) m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture(m_iTextureIndex));

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}