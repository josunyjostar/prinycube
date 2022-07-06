#include "stdafx.h"
#include "StageSwapCube.h"
#include "CubeTri_Texture.h"
#include "ToolTip.h"

static bool bNight = false;

CStageSwapCube::CStageSwapCube()
:m_bAction(false)
,m_fActionDelay(0.f)
,m_bReset(false)
,m_bNight(false)
{
}

CStageSwapCube::~CStageSwapCube()
{
	this->Clear();
}

void CStageSwapCube::SetBackGround(RCP_BACKGROUND _pBackGround, const _ushort& _wTexIdx, const _char* _pTexTag, const _ushort& _wTargetTexIdx, const _char* _pTargetTexTag)
{
	m_pBackGround = _pBackGround;
	m_pBackGround->SetTexture(_wTexIdx, _pTexTag);

	m_wTexID = _wTexIdx;
	m_pTexTag = _pTexTag;

	m_wTargetTexID = _wTargetTexIdx;
	m_pTargetTexTag = _pTargetTexTag;
}

void CStageSwapCube::SwapReset()
{
	LIST_CUBE::iterator Iter = m_CubeList.begin();
	LIST_CUBE::iterator EndIter = m_CubeList.end();

	while(Iter != EndIter)
	{
		(*Iter)->SetHide((*Iter)->GetStartHide());
		Iter++;
	}

	m_pBackGround->SwitchTexture(m_wTargetTexID, m_pTargetTexTag, 1.5f);
	Swap(m_wTexID, m_wTargetTexID);
	Swap(m_pTexTag, m_pTargetTexTag);
	m_bReset = false;
	bNight = false;
}

HRESULT CStageSwapCube::Init()
{
	m_pResource = AddComponent<Engine::CCubeTri_Texture>();
	m_pResource->LoadResource<Engine::CCubeTri_Texture>(RESOURCE_STATIC, "CUBETRITEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_WHITE");

	m_pTransform->SetScaleY(0.5f);
	m_pTransform->Rotate(_vec3(0.f, 1.f, 0.f), PI*0.25f);

	return S_OK;
}

void CStageSwapCube::Update()
{
	CBasicCube::Update();

	if(m_pPlayer->CubeSwapResetComplete() && m_bReset) 
	{
		this->SwapReset();
	}

	float dt = m_pTimer->GetFrameTime();


	if(!m_bAction) {
		if(Engine::KeyInput->IsDown(VK_NUMPAD5))
		{
			_vec3 vPlayerPos = m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition();
			_vec3 vPos = m_pTransform->GetPosition();

			if((vPos.y < vPlayerPos.y) && sqrDistance(vPos, vPlayerPos) < 1.f)
			{
				Engine::SoundMgr->Play(SND_SWAP);
				m_bAction = true;
				m_fActionDelay = 0.f;
				bNight = !bNight;
				m_bNight = bNight;
				this->StageSwap();
			}
		}
	}
	else {
		m_fActionDelay += dt;
		if(m_fActionDelay > 2.f) m_bAction = false;
	}

	if(bNight) {
		g_fTime -= dt;
		if(g_fTime < 0.f) g_fTime = 0.f;
	}
	else {
		g_fTime += dt;
		if(g_fTime > 1.f) g_fTime = 1.f;
	}
	g_fLightAmount = g_fTime * 0.4f + 0.6f;
}

void CStageSwapCube::StageSwap()
{
	LIST_CUBE::iterator Iter = m_CubeList.begin();
	LIST_CUBE::iterator EndIter = m_CubeList.end();

	while(Iter != EndIter)
	{
		(*Iter)->SetHide(!(*Iter)->GetHide());
		Iter++;
	}

	m_pBackGround->SwitchTexture(m_wTargetTexID, m_pTargetTexTag, 1.5f);
	Swap(m_wTexID, m_wTargetTexID);
	Swap(m_pTexTag, m_pTargetTexTag);
	m_bReset = true;
}

void CStageSwapCube::Clear()
{
	m_CubeList.clear();
}