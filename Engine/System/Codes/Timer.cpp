#include "Timer.h"
using namespace Engine;

CTimer::CTimer()
:m_fTimeDelta(0.f)
,m_fFixTime(0.f)
,m_fLastIntervalTime(0.f)
,m_fFrameDelay(0.f)
,m_fCumTime(0.f)
,m_fFrameTimeLimit(0.f)
,m_bStop(false)
,m_bTimeSync(true)
{
}

CTimer::~CTimer()
{
}

bool CTimer::IsFrameReady()
{
	if(m_fCumTime > m_fFrameDelay) {
		m_bFrameReady = true;
		
		if(m_bTimeSync && m_fCumTime > m_fFrameTimeLimit)
		{
			m_fLastIntervalTime = m_fCumTime;
			m_fCumTime = 0.f;
		}
		else 
		{
			m_fLastIntervalTime = m_fFrameDelay;
			m_fCumTime -= m_fFrameDelay;
		}
	}
	return m_bFrameReady;
}


HRESULT CTimer::Init()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceFrequency(&m_CPUTick);
	m_LastTime = m_FrameTime;
	m_fFixTime = m_fTimeDelta = 0.f;

	return S_OK;
}

void CTimer::Update()
{
	QueryPerformanceCounter(&m_FrameTime);
	m_fTimeDelta = (_float(m_FrameTime.QuadPart) - _float(m_LastTime.QuadPart)) / _float(m_CPUTick.QuadPart);
	m_fFixTime += m_fTimeDelta;
	m_LastTime = m_FrameTime;
	
	if(m_fFixTime > CPUTICK_UPDATE_INTERVAL) {
		QueryPerformanceFrequency(&m_CPUTick);
		m_fFixTime = 0.f;
	}

	m_fCumTime += m_fTimeDelta;
	m_bFrameReady = false;
}