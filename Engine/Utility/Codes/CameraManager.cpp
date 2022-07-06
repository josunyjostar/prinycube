#include "CameraManager.h"
using namespace Engine;

CCameraManager::CCameraManager()
:m_hCamera(32)
{
}

CCameraManager::~CCameraManager()
{
	this->Release();
}

void CCameraManager::Update(const _float& _fDeltaTime)
{
	m_pMainCam->Update(_fDeltaTime);
	m_pMainCam->SetTransform();
}

void CCameraManager::Release()
{
	m_pDev = NULL;
	m_pMainCam = NULL;
	m_hCamera.Clear(true);
}