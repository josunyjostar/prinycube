#ifndef Timer_h__
#define Timer_h__

#include "Engine_Include.h"
#include "Base.h"

#define CPUTICK_UPDATE_INTERVAL	1.f

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	DeclRTTI(CTimer, CBase);

	void SetFrameDelay(_float _fFrameDelay)	{ m_fFrameDelay = _fFrameDelay; m_fFrameTimeLimit = m_fFrameDelay*5.f; }
	void SetFrameStop(_bool _bStop)			{ m_bStop = _bStop; }
	void SetTimeSync(_bool _bSync)			{ m_bTimeSync = _bSync; }

	_float GetTime() const		{ return m_fTimeDelta; }
	_float GetFrameTime() const	{ return (m_bStop)?0.f:m_fLastIntervalTime; }
	_bool GetFrameStop() const	{ return m_bStop; }

	_bool IsFrameReady();

public:
	HRESULT Init();
	void	Update();

private:
	LARGE_INTEGER	m_FrameTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CPUTick;
	_float			m_fFixTime;
	_float			m_fTimeDelta;

	_float			m_fLastIntervalTime;
	_float			m_fFrameDelay;
	_float			m_fCumTime;
	_float			m_fFrameTimeLimit;
	_bool			m_bFrameReady;
	_bool			m_bStop;
	_bool			m_bTimeSync;

public:
	CREATE(CTimer);
};

END

TYPEDEF_RCP(Engine::CTimer, TIMER);

#endif // Timer_h__
