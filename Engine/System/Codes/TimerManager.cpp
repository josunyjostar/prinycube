#include "TimerManager.h"
using namespace Engine;

CTimerManager::CTimerManager()
:m_hTimers(256)
{
}

CTimerManager::~CTimerManager()
{
	this->Release();
}

RCP_TIMER CTimerManager::CreateTimer(const _char* _szTimerName)
{
	RCP_TIMER& pTimer = m_hTimers[_szTimerName];

	if(!pTimer)	pTimer = CTimer::Create();

	return pTimer;
}

void CTimerManager::Release()
{
	m_hTimers.Clear(true);
}