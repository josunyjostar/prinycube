inline void Utility_Release()
{
	CLightManager::Inst().Release();
	CCameraManager::Inst().Release();
	CCallbackManager::Inst().Release();
	CSceneManager::Inst().Release();
	CRenderManager::Inst().Release();
	CCollisionManager::Inst().Release();
	CSoundManager::Inst().Release();
}