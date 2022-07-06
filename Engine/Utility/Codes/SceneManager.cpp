#include "SceneManager.h"
using namespace Engine;

CSceneManager::CSceneManager()
:m_hScene(32)
,m_rcpRunningScene(NULL)
,m_pSceneName(NULL)
,m_pNextSceneName(NULL)
,m_pNextScene(NULL)
,m_bPrevSceneClear(false)
,m_bSwapScene(false)
,m_fFadeOutTime(0.f)
{
}

CSceneManager::~CSceneManager()
{
	this->Release();
}

void CSceneManager::Update(float _fDeltaTime)
{
	if(m_rcpRunningScene)
		m_rcpRunningScene->Update();

	if(m_bSwapScene) {
		m_fFadeOutTime -= _fDeltaTime;
		if(m_fFadeOutTime < 0.f) {
			m_bSwapScene = false;

			m_hScene[m_pSceneName]->SetActive(false);
			if(m_bPrevSceneClear) m_hScene[m_pSceneName] = NULL;

			m_rcpRunningScene = *m_pNextScene;
			m_pSceneName = m_pNextSceneName;
			(*m_pNextScene)->SetActive(true);
		}
	}
}
RCP_SCENE CSceneManager::StartScene(const _char* _pSceneName, _float _fFadeOutTime /*= 0.f*/, _bool _bPrevSceneClear /*= true*/)
{
	if(m_bSwapScene) return NULL;

	RCP_SCENE& pScene = m_hScene[_pSceneName];
	if(!pScene) return NULL;

	return StartScene(pScene, _pSceneName, _fFadeOutTime, _bPrevSceneClear);
}

RCP_SCENE CSceneManager::StartScene(RCP_SCENE& _pScene, const _char* _pSceneName, _float _fFadeOutTime /*= 0.f*/, _bool _bPrevSceneClear /*= true*/)
{
	if(m_rcpRunningScene)
	{
		m_bSwapScene = true;
		m_fFadeOutTime = _fFadeOutTime;
		m_bPrevSceneClear = _bPrevSceneClear;
		m_pNextSceneName = _pSceneName;
		m_pNextScene = &_pScene;
	}
	else {
		m_rcpRunningScene = _pScene;
		m_pSceneName = _pSceneName;
		_pScene->SetActive(true);
	}

	return _pScene;
}

void CSceneManager::Release()
{
	m_hScene.Clear(true);
	m_rcpRunningScene = NULL;
	m_pSceneName = NULL;
}