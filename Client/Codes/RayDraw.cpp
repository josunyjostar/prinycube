#include "stdafx.h"
#include "RayDraw.h"

CRayDrawing::CRayDrawing()
: m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTransform(AddComponent<Engine::CTransform>())
,m_pTexture(AddComponent<Engine::CTexture>())
,m_pColorShader(Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX"))
,m_vColor(1,1,0,0.9f)
{
}

CRayDrawing::~CRayDrawing()
{

}


void CRayDrawing::SetDir( const _vec3& _vDir )
{ 
	_vec3 vUp;
	D3DXVec3Cross(&vUp, &_vDir, &_vec3(0.f, 1.f, 0.f));
	m_pTransform->LookAt(_vDir, vUp);
}

void CRayDrawing::SetRay( const _ray& _Ray )
{
	m_pTransform->SetPosition(_Ray.Pos);
	_vec3 vUp;
	D3DXVec3Cross(&vUp, &_Ray.Dir, &_vec3(1.f, 0.f, 0.f));
	m_pTransform->LookAt(_Ray.Dir, vUp);
	m_pTransform->Rotate(m_pTransform->GetUpVector(), PI*0.5f);
	_vec3 vRight = m_pTransform->GetRightVector();

	_vec3 tempN, tempN2;
	_vec3 CameraLook = Engine::CCameraManager::Inst().GetMainCam()->GetLookVector();
	_float dot = D3DXVec3Dot(D3DXVec3Normalize(&tempN, &m_pTransform->GetUpVector()), D3DXVec3Normalize(&tempN2, &CameraLook));
	m_pTransform->Rotate(m_pTransform->GetRightVector(), -dot);
}

void CRayDrawing::SetRayEnd( const _vec3& _vPos )
{
	_vec3 vPos, vEnd;
	vPos = m_pTransform->GetPosition();
	vEnd = _vPos - vPos;
	_float fLength = D3DXVec3Length(&vEnd);
	m_pTransform->SetScaleX(fLength);
	m_pTransform->AddPosition(vEnd/2.f);
}


void CRayDrawing::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, int _TexIndex /*= 0*/)
{
	m_pTexture->LoadResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

HRESULT CRayDrawing::Init()
{
	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_RAY");

	return S_OK;
}

void CRayDrawing::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pColorShader->SetVector("vColor", &m_vColor);
	m_pColorShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pColorShader->SetMatrix("matView", &matView);
	m_pColorShader->SetMatrix("matProj", &matProj);
	m_pColorShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

	m_pColorShader->Begin(NULL, NULL);
	m_pColorShader->BeginPass(0);

	m_pResource->Render();

	m_pColorShader->EndPass();
	m_pColorShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}