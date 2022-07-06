#include "stdafx.h"
#include "Player.h"

#include "Dust.h"
#include "Dove.h"
#include "FootPrint.h"
#include "AfterImage.h"
#include "Dead.h"
#include "Splash.h"
#include "Arrow.h"

#include "MoveCube.h"
#include "BridgeCube.h"
#include "PickUpCube.h"
#include "StageSwapCube.h"
#include "Ocean.h"
#include "TempleLight.h"
#include "UI.h"
#include "DeadWater.h"
#include "MirrorCube.h"

#include "ToolTip.h"

CPlayer::CPlayer()
:pResourceMgr(&Engine::CResourceManager::Inst())
,m_fDeltaTime(0.f)
,m_eViewDir(DIR_LB)
,m_ePrevViewDir(DIR_LB)
,m_eState(ANI_STAND)
,m_ePrevState(ANI_STAND)
,m_bJump(false)
,m_fMoveSpeed(1.f)
,m_fVelocity(0.f)
,m_fJumpPower(0.f)
,m_bHide(false)
,m_fHideVal(0.f)
,m_bWin(false)
,m_fSwitchFoot(1.f)
,m_eOldState(ANI_STAND)
,m_bArrowPush(false)
,m_fPushDelay(0.f)
,m_bPickUp(false)
,m_bThrow(false)
,m_bCubeSwapReset(false)
,m_vSpawnPos(_vec3(0.f,1.f,0.f))
{
}

CPlayer::~CPlayer()
{
	StopRepeat("EffectAfterImage");
}

void CPlayer::SetShadow(RCP_STAGESCENE _pStageScene, RCP_SHADOWLIGHT _pShadowLight)
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
}

HRESULT CPlayer::Init()
{
	m_pTimer = Engine::CTimerManager::Inst().FindTimer("MainApp_Frame");
	m_pGraphicDev = Engine::CGraphicDev::Inst().GetDev();
	m_pTransform = this->AddComponent<Engine::CTransform>();
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);

	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	m_pResource->SetOffset(0.5f, 0.f);

	m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_ALPHATEX");
	m_pShadowCreateShader = Engine::CShaderManager::Inst().FindShader("SHADER_SHADOWCREATE");

	m_pAnimator = this->AddComponent<CAnimator>();
	m_pAnimator->SetResource((RCP_RESOURCE)m_pResource);
	m_pAnimator->SetPlaySpeed(1.5f);
	m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_STAND_LB");

	this->AddComponent<Engine::CCubeCollider>()->SetSize(_vec3(0.15f, 1.0f, 0.15f));

	m_PrevPosition = _vec3(0.f, 1.f, 0.f);
	m_pTransform->SetPosition(m_PrevPosition);
	m_pTransform->LookAt(_vec3(1.f, 0.f, 1.f), _vec3(0.f, 1.f, 0.f));
	m_fMoveSpeed = 1.5f;

	m_bJump = false;
	m_fVelocity = 0.f;
	m_fJumpPower = 0.f;

	AddCallbackFunc(CPlayer, EffectAfterImage);

	return S_OK;
}

void CPlayer::Update()
{
	m_fDeltaTime = m_pTimer->GetFrameTime();
	m_PrevPosition = m_pTransform->GetPosition();

	this->KeyInput();
	this->Animation();

	this->EffectFootPrint();

	if(m_fDeltaTime != 0.f) {
		m_fVelocity += GRAVITY_VELOCITY * m_fDeltaTime;
		if(m_fJumpPower > 0)
			m_fJumpPower -= m_fVelocity * 125.f * m_fDeltaTime;
		else
		{
			m_bJump = true;
			m_fJumpPower -= m_fVelocity * 62.5f * m_fDeltaTime;
		}
		
		m_pTransform->AddPositionY(m_fJumpPower * m_fDeltaTime);
	}

	if(m_fJumpPower < -50.f) {
		m_fJumpPower = 0;
		m_fVelocity = 0;
		m_bJump = false;
		m_bArrowPush = false;
		m_bCubeSwapReset = true;
		m_pTransform->SetPosition(m_vSpawnPos);
		Engine::SoundMgr->Play(SND_DEAD);
	}

	if(m_bArrowPush)
	{
		m_pTransform->AddPosition(m_vPushScalar * m_fPushDelay * m_fDeltaTime);
		m_fPushDelay -= 5.f * m_fDeltaTime;
		if(m_fPushDelay < 0)
			m_bArrowPush = false;
	}

	this->EffectDust();
}

void CPlayer::Render()
{
	_vec3 vScale = m_pTransform->GetScale();
	_vec3 vPosition = m_pTransform->GetPosition()-_vec3(0.f, 0.5f - m_fHideVal, 0.f);
	_matrix matWorld;

	if(m_pShadowLight)
	{
		_matrix matLightWorld, matLightView;
		matLightView = m_pShadowLight->GetLightView();
		D3DXMatrixInverse(&matLightWorld, NULL, &matLightView);
		matWorld = _matrix(vScale.x,0,0,0, 0,vScale.y,0,0, 0,0,vScale.z,0, 0,0,0,1)*matLightWorld;
		memcpy(matWorld.m[3], &(vPosition + _vec3(0.f, -0.1f, 0.f)), sizeof(_vec3));

		LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
		LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
		LPDIRECT3DSURFACE9 pShadowSurface = NULL;

		m_pGraphicDev->GetRenderTarget(0, &pHWBackBuffer);
		m_pGraphicDev->GetDepthStencilSurface(&pHWDepthStencilBuffer);

		m_pShadowCreateShader->SetFloat("fAlpha", 1.f);
		m_pShadowCreateShader->SetMatrix("matWorld", &matWorld);
		m_pShadowCreateShader->SetMatrix("matLightView", &matLightView);
		m_pShadowCreateShader->SetMatrix("matLightProj", &m_matLightProj);
		m_pShadowCreateShader->SetTexture("DiffuseMap", m_pPlayTexture->GetTexture(m_pAnimator->GetFrame()));

		m_pShadowCreateShader->Begin(NULL, NULL);
		m_pShadowCreateShader->BeginPass(0);

		for(int i = 0; i < WALL_END; ++i)
		{
			m_pShadowRenderTarget[i]->GetSurfaceLevel(0, &pShadowSurface);
			m_pGraphicDev->SetRenderTarget(0, pShadowSurface);
			m_pGraphicDev->SetDepthStencilSurface(&*m_pShadowDepthStencil[i]);

			m_pAnimator->Render(0.f, true);

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

	matWorld = MoveMatrix(_matrix(vScale.x,0,0,0, 0,1.0926050f*vScale.y,0,0, 0,0,vScale.z,0, 0,0,0,1), vPosition);

	m_pShader->SetFloat("fAlpha", m_fHideVal);
	m_pShader->SetMatrix("matWorld", &matWorld);
	m_pShader->SetMatrix("matView", &matView);
	m_pShader->SetMatrix("matProj", &matProj);
	m_pShader->SetTexture("DiffuseMap", m_pPlayTexture->GetTexture(m_pAnimator->GetFrame()));

	m_pShader->Begin(NULL, NULL);
	m_pShader->BeginPass(0);

	m_pAnimator->Render(m_fDeltaTime, true);

	m_pShader->EndPass();
	m_pShader->End();
}

void CPlayer::Collision(Engine::COL_INFO& _ColInfo)
{
	if(IsDynamicCast(CTempleLight, _ColInfo.pObj)) return;

	RCP_TRANSFORM pColObjTransform = _ColInfo.pObj->GetComponent<Engine::CTransform>();
	_vec3 ColObjectPosition = pColObjTransform->GetPosition();

	float minDist = (pColObjTransform->GetScale().y * _ColInfo.pObj->GetComponent<Engine::CCubeCollider>()->GetSize().y
		+ m_pTransform->GetScale().y * this->GetComponent<Engine::CCubeCollider>()->GetSize().y) * 0.5f;

	if(IsDynamicCast(COcean, _ColInfo.pObj))
	{
		_vec3 vMotion = ((RCP_MOVECUBE)_ColInfo.pObj)->GetMotionVector();
		if(m_pTransform->GetPosition().y - ColObjectPosition.y - minDist >= -0.5f)
		{
			m_fJumpPower = 0;
			m_fVelocity = 0;
			m_bJump = false;

			RCP_DEAD pDead = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("Dead")->Add_GameObject<CDead>(true);
			
			RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");
			pLayer->Add_GameObject<CSplash>()->SetCreatePos(m_PrevPosition);

			int iIndex;
			((RCP_OCEAN)Engine::CSceneManager::Inst().GetRunningScene()->GetLayer("Tile")->Find_GameObject("COcean"))->GetFace(iIndex, m_pTransform->GetPosition());
			pDead->SetIndex(iIndex);
			_vec3 P, N;
			((RCP_OCEAN)Engine::CSceneManager::Inst().GetRunningScene()->GetLayer("Tile")->Find_GameObject("COcean"))->GetPosAndNormal(P, N, iIndex);
			pDead->GetComponent<Engine::CTransform>()->SetPosition(P);

			switch(m_eViewDir)
			{
			case DIR_LB: pDead->SetTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_LB", DIR_LB);
				pLayer->Add_GameObject<CDeadWater>()->SetCreatePos( pDead->GetCreatePos());
				break;
			case DIR_LT: pDead->SetTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_LT", DIR_LT);	break;
			case DIR_RB: pDead->SetTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_RB", DIR_RB);
				pLayer->Add_GameObject<CDeadWater>()->SetCreatePos( pDead->GetCreatePos());
				break;
			case DIR_RT: pDead->SetTexture(RESOURCE_STATIC, "TEX_PRINNY_DEAD_RT", DIR_RT);	break;
			}

			m_pTransform->SetPosition(m_vSpawnPos);
			m_bCubeSwapReset = true;
			return;
		}
	}
	else
		m_bCubeSwapReset = false;

	if(m_fJumpPower > 0.f && ColObjectPosition.y > m_PrevPosition.y)
	{
		if((ColObjectPosition.y - m_PrevPosition.y) >= minDist)
		{
			m_pTransform->SetPositionY(ColObjectPosition.y-minDist-0.000125f);
			m_fJumpPower = 0;
			return;
		}
	}
	else if(m_fJumpPower <= 0.f)
	{
		if(IsDynamicCast(CMoveCube, _ColInfo.pObj))
		{
			_vec3 vMotion = ((RCP_MOVECUBE)_ColInfo.pObj)->GetMotionVector();
			if((m_PrevPosition.y - ColObjectPosition.y + vMotion.y) >= minDist-0.3f)
			{
				m_fJumpPower = 0;
				m_fVelocity = 0;
				m_bJump = false;
				m_pTransform->SetPositionY(ColObjectPosition.y + minDist+0.000125f);
				m_pTransform->AddPosition(vMotion);
				return;
			}
		}
		else if((m_PrevPosition.y - ColObjectPosition.y) >= minDist-0.000125f)
		{
			m_fJumpPower = 0;
			m_fVelocity = 0;
			m_bJump = false;
			m_pTransform->SetPositionY(ColObjectPosition.y + minDist + 0.000125f);
			return;
		}
	}

	m_pTransform->SetPositionX(m_PrevPosition.x);
	m_pTransform->SetPositionZ(m_PrevPosition.z);

	if(!IsDynamicCast(CArrow, _ColInfo.pObj))
		m_bArrowPush = false;
}

void CPlayer::CollisionExit(Engine::COL_INFO& _ColInfo)
{
	if(!IsDynamicCast(CMoveCube, _ColInfo.pObj) || m_fJumpPower > 0.f) return;

	RCP_TRANSFORM pColObjTransform = _ColInfo.pObj->GetComponent<Engine::CTransform>();
	_vec3 ColObjectPosition = pColObjTransform->GetPosition();

	float minDist = (pColObjTransform->GetScale().y * _ColInfo.pObj->GetComponent<Engine::CCubeCollider>()->GetSize().y
		+ m_pTransform->GetScale().y * this->GetComponent<Engine::CCubeCollider>()->GetSize().y) * 0.5f;

	if(ColObjectPosition.y < m_PrevPosition.y)
	{
		_vec3 vMotion = ((RCP_MOVECUBE)_ColInfo.pObj)->GetMotionVector();

		if((m_PrevPosition.y - ColObjectPosition.y + vMotion.y) >= minDist)
		{
			m_fJumpPower = 0;
			m_fVelocity = 0;
			m_bJump = false;
			m_pTransform->SetPositionY(ColObjectPosition.y + minDist - 0.000125f);
		}
	}
}

void CPlayer::KeyInput()
{
	if(m_fDeltaTime == 0.f) return;

	if(m_bArrowPush) return;
	if(m_bThrow) {
		m_eState = ANI_THROW;
		if(m_pAnimator->IsPlay())
			return;
		else m_bThrow = false;
	}

	m_eState = ANI_STAND;

	if(Engine::KeyInput->Is('W'))
	{
		m_bWin = true;
		m_eState = ANI_WIN;
	}
	else m_bWin = false;
	if(Engine::KeyInput->Is(VK_NUMPAD1))
	{
		m_eState = ANI_WALK;
		m_eViewDir = DIR_LB;
		m_pTransform->LookAt(_vec3(1.f, 0.f, 1.f), _vec3(0.f, 1.f, 0.f));
		m_pTransform->AddPosition(_vec3(-1.f, 0.f, -1.f)*(m_fDeltaTime*m_fMoveSpeed));
	}

	else if(Engine::KeyInput->Is(VK_NUMPAD3))
	{
		m_eState = ANI_WALK;
		m_eViewDir = DIR_RB;
		m_pTransform->LookAt(_vec3(-1.f, 0.f, 1.f), _vec3(0.f, 1.f, 0.f));
		m_pTransform->AddPosition(_vec3(1.f, 0.f, -1.f)*(m_fDeltaTime*m_fMoveSpeed));
	}

	else if(Engine::KeyInput->Is(VK_NUMPAD7))
	{
		m_eState = ANI_WALK;
		m_eViewDir = DIR_LT;
		m_pTransform->LookAt(_vec3(-1.f, 0.f, 1.f), _vec3(0.f, 1.f, 0.f));
		m_pTransform->AddPosition(_vec3(-1.f, 0.f, 1.f)*(m_fDeltaTime*m_fMoveSpeed));
	}

	else if(Engine::KeyInput->Is(VK_NUMPAD9))
	{
		m_eState = ANI_WALK;
		m_eViewDir = DIR_RT;
		m_pTransform->LookAt(_vec3(1.f, 0.f, 1.f), _vec3(0.f, 1.f, 0.f));
		m_pTransform->AddPosition(_vec3(1.f, 0.f, 1.f)*(m_fDeltaTime*m_fMoveSpeed));
	}

	if(Engine::KeyInput->IsDown(VK_SPACE) && !m_bJump)
	{
		m_bJump = true;
		m_fJumpPower = JUMP_POWER;
		m_fVelocity = 0.f;
	}

	if(m_bJump) {
		if(m_fJumpPower > 0)	m_eState = ANI_JUMP_UP;
		else					m_eState = ANI_JUMP_DOWN;
	}

	static RCP_PICKUPCUBE pPickCube;
	if(m_bPickUp)
	{
		if(!m_bJump && m_eState == ANI_WALK) {
			static float fSndDelay = 0.15f;
			fSndDelay-=m_fDeltaTime;
			if(fSndDelay < 0.f) {
				fSndDelay = 0.15f;
				Engine::SoundMgr->Play(SND_TILE_CLICK);
				Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer")->Add_GameObject<CDust>(true);
			}
		}

		if(m_eState == ANI_STAND)	m_eState = ANI_PICKUP;
		else						m_eState = ANI_ROTATE;

		if(Engine::KeyInput->IsDown(VK_NUMPAD8))
		{
			m_bPickUp = false;
			m_eState = ANI_THROW;
			m_bThrow = true;
			_vec3 vThrowPos;
			switch(m_eViewDir)
			{
			case DIR_LB: vThrowPos = VEC_LB(1.1f); break;
			case DIR_LT: vThrowPos = VEC_LT(1.1f); break;
			case DIR_RB: vThrowPos = VEC_RB(1.1f); break;
			case DIR_RT: vThrowPos = VEC_RT(1.1f); break;
			}
			vThrowPos += m_pTransform->GetPosition();

			pPickCube->Throw(vThrowPos);
			Engine::SoundMgr->Play(SND_THROW);
		}
	}
	else
	{
		if(Engine::KeyInput->IsDown(VK_NUMPAD8))
		{
			_ray rRay;
			rRay.Pos = m_pTransform->GetPosition();
			switch(m_eViewDir)
			{
			case DIR_LB: rRay.Dir = VEC_LB(1.1f); break;
			case DIR_LT: rRay.Dir = VEC_LT(1.1f); break;
			case DIR_RB: rRay.Dir = VEC_RB(1.1f); break;
			case DIR_RT: rRay.Dir = VEC_RT(1.1f); break;
			}
			Engine::COL_INFO ColInfo = Engine::RayCast->Picking(rRay, "PickCube");

			if(ColInfo.pObj && sqrDistance(ColInfo.vColPos, m_pTransform->GetPosition()) <= 2.f) {
				m_bPickUp = true;
				pPickCube = ColInfo.pObj;
				pPickCube->Pick(this);
			}
			else
			{
				rRay.Dir.y += -0.15f;
				Normalize(rRay.Dir);

				ColInfo = Engine::RayCast->Picking(rRay, "Mirror");

				if(ColInfo.pObj && sqrDistance(ColInfo.vColPos, m_pTransform->GetPosition()) <= 2.f) {
					if(((CMirrorCube*)ColInfo.pObj)->GetCanPick())
					{
						m_bPickUp = true;
						pPickCube = ColInfo.pObj;
						pPickCube->Pick(this);
					}
				}
			}
		}
	}

	if (Engine::KeyInput->IsDown(VK_CONTROL))
	{
		m_fMoveSpeed = 2.5f;
		Repeat("EffectAfterImage", 0.1f);
	}
	else if (Engine::KeyInput->IsUp(VK_CONTROL))
	{
		m_fMoveSpeed = 1.5f;
		StopRepeat("EffectAfterImage");
	}

	if(m_pShadowLight)
	{
		if( m_eViewDir == DIR_RT || m_eViewDir == DIR_LB )
			m_pShadowLight->SetLightDir(true);
		else
			m_pShadowLight->SetLightDir(false);
	}
}

void CPlayer::Animation()
{
	if(m_ePrevState != m_eState || m_ePrevViewDir != m_eViewDir)
	{
		if(m_ePrevState == ANI_JUMP_DOWN && m_ePrevViewDir == m_eViewDir) Engine::SoundMgr->Play(SND_JUMP_DOWN);

		switch(m_eState)
		{
		case ANI_STAND:
			m_pAnimator->SetPlaySpeed(1.5f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_STAND_LB");	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_STAND_LT");	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_STAND_RB");	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_STAND_RT");	break;
			}
			break;

		case ANI_WALK:
			m_pAnimator->SetPlaySpeed(4.f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WALK_LB");	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WALK_LT");	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WALK_RB");	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WALK_RT");	break;
			}
			break;

		case ANI_JUMP_UP:
			m_pAnimator->SetPlaySpeed(1.f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_LB");	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_LT");	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_RB");	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_UP_RT");	break;
			}
			if(m_ePrevViewDir == m_eViewDir) Engine::SoundMgr->Play(SND_JUMP_UP);
			break;

		case ANI_JUMP_DOWN:
			m_pAnimator->SetPlaySpeed(1.f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_LB");	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_LT");	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_RB");	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_JUMP_DOWN_RT");	break;
			}
			break;

		case ANI_WIN:
			m_pAnimator->SetPlaySpeed(2.0f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WIN_LB", false);	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WIN_LT", false);	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WIN_RB", false);	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_WIN_RT", false);	break;
			}
			break;
		case ANI_PICKUP:
			m_pAnimator->SetPlaySpeed(4.f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_LB");	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_LT");	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_RB");	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_PICKUP_RT");	break;
			}
			break;
		case ANI_THROW:
			m_pAnimator->SetPlaySpeed(3.f);
			switch(m_eViewDir)
			{
			case DIR_LB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_THROW_LB", false);	break;
			case DIR_LT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_THROW_LT", false);	break;
			case DIR_RB: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_THROW_RB", false);	break;
			case DIR_RT: m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_THROW_RT", false);	break;
			}
			break;
		case ANI_ROTATE:
			m_pAnimator->SetPlaySpeed(1.f);
			m_pPlayTexture = m_pAnimator->Play(RESOURCE_STATIC, "TEX_PRINNY_ROTATION", true, false);
			break;
		}

		m_ePrevState = m_eState;
		m_ePrevViewDir = m_eViewDir;
	}
}

void CPlayer::EffectFootPrint()
{
	if (m_eState != ANI_WALK)
		return;

	if (Engine::KeyInput->Is(VK_NUMPAD1) || Engine::KeyInput->Is(VK_NUMPAD3) || Engine::KeyInput->Is(VK_NUMPAD7) || Engine::KeyInput->Is(VK_NUMPAD9))
	{
		RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");
		static float fEffectTime = -1.f;

		fEffectTime += (m_fDeltaTime * (10.f));

		if (0 < fEffectTime)
		{
			pLayer->Add_GameObject<CFootPrint>(true);
			fEffectTime = -1;
		}
	}
}

void CPlayer::EffectDust()
{
	if (ANI_JUMP_DOWN == m_eState )
		m_eOldState = ANI_JUMP_DOWN;

	if (m_eOldState == ANI_JUMP_DOWN )
	{
		if (m_eState == ANI_STAND || m_eState == ANI_WALK)
		{
			m_eOldState = m_eState;
			RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");

			pLayer->Add_GameObject<CDust>(true);
			pLayer->Add_GameObject<CDust>(true);
			pLayer->Add_GameObject<CDust>(true);
			pLayer->Add_GameObject<CDust>(true);
		}
	}
}

void CPlayer::EffectAfterImage( void* pData )
{
	if (Engine::KeyInput->Is(VK_NUMPAD1) || Engine::KeyInput->Is(VK_NUMPAD3) || Engine::KeyInput->Is(VK_NUMPAD7) 
		|| Engine::KeyInput->Is(VK_NUMPAD9) || Engine::KeyInput->Is(VK_SPACE))
	{
		RCP_LAYER pLayer = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer");
		pLayer->Add_GameObject<CAfterIamge>()->Setting(this);

		if (m_eState == ANI_WALK)
			pLayer->Add_GameObject<CDust>(true);
	}
}