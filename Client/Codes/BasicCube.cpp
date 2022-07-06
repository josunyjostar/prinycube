#include "stdafx.h"
#include "BasicCube.h"
#include "Light.h"

CBasicCube::CBasicCube()
:m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTransform(AddComponent<Engine::CTransform>())
,m_pCollider(AddComponent<Engine::CCubeCollider>())
,m_pRenderer(AddComponent<Engine::CRenderer>())
,m_pTexture(AddComponent<Engine::CTexture>())
,m_pShadowCreateShader(Engine::CShaderManager::Inst().FindShader("SHADER_SHADOWCREATE"))
,m_pColorShader(Engine::CShaderManager::Inst().FindShader("SHADER_DIFFUSECOLORTEX"))
,m_bHide(false)
,m_fHideVal(1.f)
,m_bStartHide(false)
,m_vColor(1,1,1,1)
{
}

CBasicCube::~CBasicCube()
{
}

void CBasicCube::SetShadow(RCP_STAGESCENE _pStageScene, const _vec3& _vCreatePos, RCP_SHADOWLIGHT _pShadowLight)
{
	RCP_D3DTEXTURE* pRenderTarget	= _pStageScene->GetShadowRenderTarget();
	RCP_SURFACE* pDepthBuffer		= _pStageScene->GetShadowDepthBuffer();
	m_pShadowLight					= _pStageScene->GetShadowLight();
	m_matLightProj					= _pStageScene->GetLightProjMatrix();

	for(int i = 0; i < WALL_END; ++i)
	{
		m_pShadowRenderTarget[i] = pRenderTarget[i];
		m_pShadowDepthStencil[i] = pDepthBuffer[i];
	}

	m_pTransform->SetPosition(_vCreatePos);
}

void CBasicCube::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag)
{
	m_pTexture->LoadResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

void CBasicCube::SetLocalScale(const _vec3& _vScale)				
{
	memcpy(m_matLocalWorld.m[0], &(Normalize(*(_vec3*)m_matLocalWorld.m[0])*_vScale.x), sizeof(_vec3));
	memcpy(m_matLocalWorld.m[1], &(Normalize(*(_vec3*)m_matLocalWorld.m[1])*_vScale.y), sizeof(_vec3));
	memcpy(m_matLocalWorld.m[2], &(Normalize(*(_vec3*)m_matLocalWorld.m[2])*_vScale.z), sizeof(_vec3));
}

void CBasicCube::SetLocalRotate(const _vec3& _vAxis, const float& _fAngle)
{
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &Normalize(_vAxis), _fAngle);

	for(int i = 0; i < 3; ++i)
		D3DXVec3TransformNormal((_vec3*)m_matLocalWorld.m[i], (_vec3*)m_matLocalWorld.m[i], &matRot);
}

bool CBasicCube::GetNormalAndDest(const _ray& _vWorldRay, _vec3& vNormal, _float& fMinDest)
{
	_vec3 vBounds[2];
	_vec3 vInvRayDir;
	_matrix matInvBoxWorld;
	_vec3 vLocalRayPos;
	_vec3 vLocalRayDir;

	D3DXMatrixInverse(&matInvBoxWorld, NULL, &m_pTransform->GetWorldMatrix());
	vLocalRayPos = TransformCoord(_vWorldRay.Pos, matInvBoxWorld);
	vLocalRayDir = TransformNormal(_vWorldRay.Dir, matInvBoxWorld);

	RCP_VIBUFFER pVIBuffer = (RCP_VIBUFFER)m_pResource;

	_ulong dwFVF  = pVIBuffer->GetFVF();
	_ulong dwVBSize  = pVIBuffer->GetVBSize();
	_ulong dwVBCnt  = pVIBuffer->GetVBCnt();
	_ulong dwIBCnt  = pVIBuffer->GetIBCnt();

	char* pVertex;
	WORD* pIndex;

	RCP_VB 	m_pVB;
	RCP_IB	m_pIB;

	m_pVB = pVIBuffer->GetVB();
	m_pIB = pVIBuffer->GetIB();

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_vec3 *v0, *v1, *v2;
	_vec3 *vv0, *vv1, *vv2;

	_float fDest = 0.f;
	bool bCol = false;

	for(_uint dwIdx = 0; dwIdx < dwIBCnt;)
	{
		v0 = (_vec3*)&pVertex[pIndex[dwIdx++]*dwVBSize];
		v1 = (_vec3*)&pVertex[pIndex[dwIdx++]*dwVBSize];
		v2 = (_vec3*)&pVertex[pIndex[dwIdx++]*dwVBSize];
		if(D3DXIntersectTri(v0, v1, v2, &vLocalRayPos, &vLocalRayDir, NULL, NULL, &fDest))
		{
			if(fDest < fMinDest)
			{
				bCol = true;
				vv0 = v0;
				vv1 = v1;
				vv2 = v2;
				fMinDest = fDest;
			}
		}
	}

	if(!bCol) return false;

	m_pVB->Unlock();
	m_pIB->Unlock();

	//부모있는 큐브는 따로 처리해야함..
	vNormal = Normalize(TransformNormal(Cross((*vv1 - *vv0), (*vv2 - *vv1)), m_pTransform->GetRotationMatrix()));
	return true;
}

void CBasicCube::SetHide(bool _bHide, bool _bInit /*= false*/)
{
	if(m_bHide && !_bHide)
		m_pCollider->SetActive(!_bHide);
	if(!m_bHide && _bHide)
	{
		m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);
		m_pCollider->SetActive(!_bHide);
	}

	m_bHide = _bHide;

	if(_bInit) {
		m_bStartHide = _bHide;
		m_fHideVal = (m_bHide)?0.f:1.f;
	}
}

HRESULT CBasicCube::Init()
{
	m_pResource = this->AddComponent<Engine::CCube_Texture>();
	m_pResource->LoadResource<Engine::CCube_Texture>(RESOURCE_STATIC, "CUBETEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->SetScaleY(0.5f);
	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	return S_OK;
}

void CBasicCube::Update()
{
	float dt = m_pTimer->GetFrameTime();

	m_fHideVal += (m_bHide)?-dt:dt;
	if(m_fHideVal > 1.f) {
		m_fHideVal = 1.f;
		if(m_pRenderer->GetRenderType() == Engine::RENDER_ALPHA_BLEND) m_pRenderer->SetRenderType(Engine::RENDER_NORMAL);
	}
	else if(m_fHideVal < 0.f) m_fHideVal = 0.f;
}

void CBasicCube::Render()
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
		m_pShadowCreateShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

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
	m_pColorShader->SetVector("vColor", &(_vec4(g_fLightAmount, g_fLightAmount, g_fLightAmount+0.1f, m_fHideVal)*m_vColor));
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
