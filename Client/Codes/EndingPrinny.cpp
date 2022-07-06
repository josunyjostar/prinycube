#include "stdafx.h"
#include "EndingPrinny.h"
#include "Dust.h"

#include "ItemHeart.h"
#include "GetHeart.h"

CEndingPrinny::CEndingPrinny()
:m_pDev(Engine::CGraphicDev::Inst().GetDev())
,m_pTimer(Engine::CTimerManager::Inst().FindTimer("MainApp_Frame"))
,m_pTransform(AddComponent<Engine::CTransform>())
,m_fDeltaTime(0.f)
,m_bJump(false)
,m_fY(0.f)
,m_fVelocity(0.f)
,m_fJumpPower(0.f)
,m_iPos(1)
,m_iLife(3)
,m_bDead(false)
,m_bSceneChange(false)
{
}

CEndingPrinny::~CEndingPrinny()
{
}

HRESULT CEndingPrinny::Init()
{
	RCP_RENDERER pRenderer = this->AddComponent<Engine::CRenderer>();
	pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");

	this->AddComponent<Engine::CSphereCollider>()->SetPosition(_vec3(0,0.5f,0));

	m_pAnimator = this->AddComponent<CAnimator>();
	m_pAnimator->SetResource((RCP_RESOURCE)m_pResource);
	m_pAnimator->SetPlaySpeed(3.f);
	m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_ROTATION");

	m_pEffectLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

	return S_OK;
}

void CEndingPrinny::Update()
{
	if(!m_bSceneChange && Engine::KeyInput->IsDown(VK_RETURN)) {
		m_bSceneChange = true;
		Engine::SoundMgr->Play(SND_SCENE_CHANGE);
		RCP_FADEINOUTFILTER pFadeInOutFilter = m_pEffectLayer->Add_GameObject<CFadeOutFilter>(true);
		pFadeInOutFilter->FadeIn();
		Engine::CSceneManager::Inst().StartScene("CTitleScene", 0.5f, true);
	}

	m_fDeltaTime = m_pTimer->GetFrameTime();
	if(m_fDeltaTime != 0.f) {
		m_fVelocity += GRAVITY_VELOCITY * m_fDeltaTime;
		if(m_fJumpPower > 0)
			m_fJumpPower -= m_fVelocity*2.f;
		else
			m_fJumpPower -= m_fVelocity;

		m_fY += m_fJumpPower * m_fDeltaTime;
	}

	if(!m_bDead) {
		if(m_fY < 0.f) {
			if(m_bJump) Engine::SoundMgr->Play(SND_JUMP_DOWN);
			m_fY = 0;
			m_fVelocity = 0;
			m_fJumpPower = 0;
			m_bJump = false;
		}

		if(!m_bJump) {
			static float fSndDelay = 0.1f;
			fSndDelay-=m_fDeltaTime;
			if(fSndDelay < 0.f) {
				fSndDelay = 0.1f;
				Engine::SoundMgr->Play(SND_TILE_CLICK);
			}
		}

		if(Engine::KeyInput->IsDown(VK_NUMPAD7))
		{
			if(--m_iPos < 0) m_iPos = 0;
			else Engine::SoundMgr->Play(SND_DEAD_CLICK);
		}
		if(Engine::KeyInput->IsDown(VK_NUMPAD3))
		{
			if(++m_iPos > 2) m_iPos = 2;
			else Engine::SoundMgr->Play(SND_DEAD_CLICK);
		}
		if(!m_bJump && Engine::KeyInput->IsDown(VK_SPACE))
		{
			Engine::SoundMgr->Play(SND_JUMP_UP);
			m_bJump = true;
			m_fJumpPower = JUMP_POWER;
			m_fVelocity = 0.f;
		}

		static _vec3 vPos[3] = {_vec3(-1,0,1), _vec3(), _vec3(1,0,-1)};

		if(!m_bJump) {
			RCP_DUST pDust = m_pEffectLayer->Add_GameObject<CDust>(true);
			pDust->GetComponent<Engine::CTransform>()->SetPosition(vPos[m_iPos]+_vec3(0,-0.3f,-0.2f));
			pDust->SetMoveDir(_vec3(RAND_RANGE_F(-0.3f, 0.3f), RAND_RANGE_F(0.f, 1.f), RAND_RANGE_F(-0.3f, 0.3f))*0.016f + _vec3(-3, 0, -3)*0.016f);
		}

		m_pTransform->SetPosition(vPos[m_iPos]);
	}
	else {
		static float fDeadTime = 0.f;
		fDeadTime += m_fDeltaTime;

		if(!m_bSceneChange && fDeadTime > 3.f) {
			m_bSceneChange = true;
			Engine::SoundMgr->Play(SND_SCENE_CHANGE);
			RCP_FADEINOUTFILTER pFadeInOutFilter = m_pEffectLayer->Add_GameObject<CFadeOutFilter>(true);
			pFadeInOutFilter->FadeIn();
			Engine::CSceneManager::Inst().StartScene("CTitleScene", 0.5f, true);
		}
	}

	m_pTransform->SetPositionY(m_fY);
}

void CEndingPrinny::Render()
{
	_matrix matWorld, matView;
	m_pDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	matWorld = m_pTransform->GetWorldMatrix() * matView;
	memcpy(matWorld.m[3], &m_pTransform->GetPosition(), sizeof(_vec3));

	m_pDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pAnimator->Render(m_fDeltaTime);
}

void CEndingPrinny::CollisionEnter(Engine::COL_INFO& _ColInfo)
{
	if(m_bDead) return;

	if(IsDynamicCast(CItemHeart, _ColInfo.pObj))
	{
		Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer")->Add_GameObject<CGetHeart>()->GetComponent<Engine::CTransform>()->SetPosition(m_pTransform->GetPosition());
		_ColInfo.pObj->SetActive(false);
		if(++m_iLife > 3) m_iLife = 3;
	}
	else
	{
		Engine::CCameraManager::Inst().GetMainCam()->Shake(0.1f);
		Engine::SoundMgr->Play(SND_STONE_HIT);
		if(--m_iLife <= 0) {
			if(Engine::SoundMgr->IsPlay(SND_WHEEL)) Engine::SoundMgr->Stop(SND_WHEEL);
			m_bJump = true;
			m_fJumpPower = JUMP_POWER;
			m_fVelocity = 0.f;
			m_bDead = true;
		}
	}
}