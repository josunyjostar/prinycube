#include "stdafx.h"
#include "Dead.h"
#include "Ocean.h"

CDead::CDead()
:m_fHideVal(0.1f)
{
}

CDead::~CDead()
{
}

void CDead::SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, VIEW_DIR _eDir)
{
	m_eDir = _eDir;
	m_pTexture->LoadResource<Engine::CTexture>(_wContainerIdx, _pResourceTag);
}

_vec3 CDead::GetCreatePos() const
{
	switch(m_eDir)
	{
	case DIR_LB: return m_pTransform->GetPosition() + _vec3(-0.2f,0.f,0.2f);
	case DIR_LT: return m_pTransform->GetPosition() + _vec3(-0.2f,0.2f,-0.2f);
	case DIR_RT: return m_pTransform->GetPosition() + _vec3(0.2f,0.2f,-0.2f);
	case DIR_RB: return m_pTransform->GetPosition() + _vec3(0.2f,0.f,0.2f);
	}
	return m_pTransform->GetPosition();
}

HRESULT CDead::Init()
{
	g_GameData.DeadCount[g_GameData.RunningStage]++;

	m_pDev = Engine::CGraphicDev::Inst().GetDev();
	m_pTransform = this->AddComponent<Engine::CTransform>();
	m_pTransform->SetScaleY(2.5f);
	m_pTransform->AddRotationX(PI*0.5f); 
	
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	m_pTexture = this->AddComponent<Engine::CTexture>();
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_PRINNY_DEAD_RB");

	m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_ALPHATEX");

	this->AddComponent<Engine::CSphereCollider>()->SetRadius(0.5f);

	Engine::SoundMgr->Play(SND_DEAD);
	Engine::SoundMgr->Play(SND_DIVE);

	return S_OK;
}

void CDead::Render()
{
	_vec3 P, N;
	((RCP_OCEAN)Engine::CSceneManager::Inst().GetRunningScene()->GetLayer("Tile")->Find_GameObject("COcean"))->GetPosAndNormal(P, N, m_iIndex);
	m_pTransform->SetPosition(P);

	_matrix matView, matProj;
	_vec3 vScale = m_pTransform->GetScale();
	_vec3 vPosition = m_pTransform->GetPosition();
	_matrix matWorld;

	matWorld = m_pTransform->GetWorldMatrix();

	m_pDev->GetTransform(D3DTS_VIEW, &matView);
	m_pDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pShader->SetFloat("fAlpha", 0.0f);
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &matView);
	m_pShader->SetMatrix("matProj", &matProj);
	m_pShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());


	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();

	m_pShader->SetFloat("fAlpha", 0.5f);

	matWorld = m_pTransform->GetWorldMatrix();

	_matrix matViewInv;
	D3DXMatrixInverse(&matViewInv, 0, &matView);
	_vec3 vView(matViewInv._31/matViewInv._32*1.5f, 1.5f,matViewInv._33/matViewInv._32*1.5f);

	m_pShader->SetMatrix("matWorld", &MoveMatrix(matWorld, vView));


	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pResource->Render();

	m_pShader->EndPass();
	m_pShader->End();



}