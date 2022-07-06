#include "stdafx.h"
#include "TempleLight.h"
#include "HeartBeat.h"
#include "StageScene.h"

CTempleLight::CTempleLight()
:m_fBright(0.f)
,m_bAction(false)
,m_pNextSceneName(NULL)
,m_bFake(false)
{
}

CTempleLight::~CTempleLight()
{
}

void CTempleLight::SetHide(bool _bHide, bool _bInit /*= false*/)
{
	if(m_bHide && !_bHide)
		m_pCollider->SetActive(!_bHide);
	if(!m_bHide && _bHide)
		m_pCollider->SetActive(!_bHide);

	m_bHide = _bHide;

	if(_bInit) m_fHideVal = (m_bHide)?0.f:1.f;
}

HRESULT CTempleLight::Init()
{
	m_pResource = this->AddComponent<Engine::CRect_Texture>();
	m_pResource->LoadResource<Engine::CRect_Texture>(RESOURCE_STATIC, "RCTEX");
	m_pTexture->LoadResource<Engine::CTexture>(RESOURCE_STATIC, "TEX_LIGHT");
	m_pRenderer->SetRenderType(Engine::RENDER_UI);
	m_pTransform->SetScale(_vec3(3.f, 3.f, 3.f));

	this->DeleteComponent<Engine::CCubeCollider>();
	m_pCollider = this->AddComponent<Engine::CSphereCollider>();
	((RCP_SPHERE)m_pCollider)->SetRadius(0.5f);

	m_pColorShader = Engine::CShaderManager::Inst().FindShader("SHADER_COLORTEX");

	m_vColor = _vec4(1,1,0.2f,1);

	return S_OK;
}

void CTempleLight::Update()
{
	float dt = m_pTimer->GetFrameTime();

	m_fBright -= dt;
	if(m_fBright < 0.f) m_fBright = 0.f;
	else if(m_fBright > 0.5f) m_fBright = 0.5f;

	m_fHideVal += (m_bHide)?-dt:dt;
	if(m_fHideVal > 1.f) m_fHideVal = 1.f;
	else if(m_fHideVal < 0.f) m_fHideVal = 0.f;

	if(!m_pNextSceneName || m_bAction) return;
	_vec3 vPlayerPos = m_pPlayer->GetComponent<Engine::CTransform>()->GetPosition();
	_vec3 vPos = m_pTransform->GetPosition();

	if((vPos.y < vPlayerPos.y) && sqrDistance(vPos, vPlayerPos) < 0.3f)
	{
		if(m_bFake)
		{
			((RCP_STAGESCENE)Engine::CSceneManager::Inst().GetRunningScene())->Reset();
			return;
		}

		m_bAction = true;

		g_GameData.Save();
		Engine::SoundMgr->Play(SND_CLEAR);

		m_pFadeInOutFilter->FadeIn(1.f, FADE_IN);
		_vec2 vPos = m_pTransform->GetScreenPos(m_pGraphicDev, WINCX, WINCY);
		m_pFadeInOutFilter->SetPosition(vPos);
		m_pFadeInOutFilter->SetColor(_vec4(1,1,0,1));
		m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));
		Engine::CSceneManager::Inst().StartScene(m_pNextSceneName, 1.f);

		Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("EffectLayer")->Add_GameObject<CHeartBeat>(true);
	}
}

void CTempleLight::Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pColorShader->SetVector("vColor", &(_vec4(1,1,1,m_fHideVal*0.5f + m_fBright)*m_vColor));
	m_pColorShader->SetMatrix("matWorld", &m_pTransform->GetWorldMatrix());
	m_pColorShader->SetMatrix("matView", &matView);
	m_pColorShader->SetMatrix("matProj", &matProj);
	m_pColorShader->SetTexture("DiffuseMap", m_pTexture->GetTexture());

	m_pColorShader->Begin(NULL, NULL);
	m_pColorShader->BeginPass(0);

	m_pResource->Render();

	m_pColorShader->EndPass();
	m_pColorShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CTempleLight::Setting( RCP_STAGESCENE _StageScene, _vec3 _StartPoint, RCP_PLAYER _Player, const char* _NextSceneName, RCP_FADEINOUTFILTER _FadeInOut, RCP_SHADOWLIGHT _ShadowLight )
{
	m_pPlayer = _Player;
	m_pNextSceneName = _NextSceneName;
	m_pFadeInOutFilter = _FadeInOut;
	this->SetShadow(_StageScene, _StartPoint, _ShadowLight);	
}