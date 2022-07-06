#include "stdafx.h"
#include "BridgeCube.h"
#include "ToolTip.h"

CBridgeCube::CBridgeCube()
:m_eDir(DIR_LT)
{
}

CBridgeCube::~CBridgeCube()
{
}

void CBridgeCube::SetViewDir(VIEW_DIR _eDir)
{
	_vec3 vPrevScale	= m_pTransform->GetScale();
	_vec3 vPrevPos		= m_pTransform->GetPosition();

	switch(m_eStartDir = m_eDir = _eDir)
	{
	case DIR_LT: m_pTransform->SetWorldMatrix(g_matLT); break;
	case DIR_LB: m_pTransform->SetWorldMatrix(g_matLB); break;
	case DIR_RB: m_pTransform->SetWorldMatrix(g_matRB); break;
	case DIR_RT: m_pTransform->SetWorldMatrix(g_matRT); break;
	}

	m_pTransform->SetScale(vPrevScale);
	m_pTransform->SetPosition(vPrevPos);
}

HRESULT CBridgeCube::Init()
{
	m_pResource = this->AddComponent<Engine::CCylinder_Texture>();
	m_pResource->LoadResource<Engine::CCylinder_Texture>(RESOURCE_STATIC, "CYLINDERTEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->SetScaleY(0.5f);
	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	return S_OK;
}

void CBridgeCube::Update()
{
	CBasicCube::Update();

	_matrix matTarget;

	switch(m_eDir)
	{
	case DIR_LT: matTarget = g_matLT; break;
	case DIR_LB: matTarget = g_matLB; break;
	case DIR_RB: matTarget = g_matRB; break;
	case DIR_RT: matTarget = g_matRT; break;
	}

	matTarget = m_pTransform->GetScaleMatrix() * MoveMatrix(matTarget, m_pTransform->GetPosition());

	m_pTransform->SetWorldMatrix(Slerp(m_pTransform->GetWorldMatrix(), matTarget, 0.1f));

	if(!m_pPlayer) return;

	if(Engine::KeyInput->IsDown(VK_NUMPAD4))
	{
		_vec3 vPlayerPos = m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition();
		_vec3 vPos = m_pTransform->GetPosition();

		if((vPos.y < vPlayerPos.y) && sqrDistance(vPos, vPlayerPos) < 1.f)
			this->LeftRotate();
	}
	if(Engine::KeyInput->IsDown(VK_NUMPAD6))
	{
		_vec3 vPlayerPos = m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition();
		_vec3 vPos = m_pTransform->GetPosition();

		if((vPos.y < vPlayerPos.y) && sqrDistance(vPos, vPlayerPos) < 1.f)
			this->RightRotate();
	}

	if(m_pPlayer->CubeSwapResetComplete())
		m_eDir = m_eStartDir;
}

void CBridgeCube::RightRotate()
{
	Engine::SoundMgr->Play(SND_ROTATION_ACTIVE);
	Engine::SoundMgr->Play(SND_ROTATION_SWING);
	switch(m_eDir)
	{
	case DIR_LT: m_eDir = DIR_LB; break;
	case DIR_LB: m_eDir = DIR_RB; break;
	case DIR_RB: m_eDir = DIR_RT; break;
	case DIR_RT: m_eDir = DIR_LT; break;
	}
}

void CBridgeCube::LeftRotate()
{
	Engine::SoundMgr->Play(SND_ROTATION_ACTIVE);
	Engine::SoundMgr->Play(SND_ROTATION_SWING);
	switch(m_eDir)
	{
	case DIR_LT: m_eDir = DIR_RT; break;
	case DIR_LB: m_eDir = DIR_LT; break;
	case DIR_RB: m_eDir = DIR_LB; break;
	case DIR_RT: m_eDir = DIR_RB; break;
	}
}