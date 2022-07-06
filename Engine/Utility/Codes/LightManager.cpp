#include "LightManager.h"
using namespace Engine;

CLightManager::CLightManager()
:m_hLight(32)
{
}

CLightManager::~CLightManager()
{
	this->Release();
}

void CLightManager::Release()
{
	m_pDev = NULL;
	m_hLight.Clear(true);
}