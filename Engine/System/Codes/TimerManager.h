#ifndef TimerManager_h__
#define TimerManager_h__

#include "TemplateSingleton.h"
#include "Timer.h"
#include "TinyHashTable.h"

BEGIN(Engine)

class ENGINE_DLL CTimerManager : public TSingleton<CTimerManager>
{
private:
	explicit CTimerManager();
	virtual ~CTimerManager();

public:
	friend TSingleton;

	RCP_TIMER	CreateTimer(const _char* _szTimerName);
	RCP_TIMER&	FindTimer(const _char* _szTimerName) { return m_hTimers[_szTimerName]; }

private:
	TinyHashTable<const _char*,RCP_TIMER>	m_hTimers;

public:
	void Release();
};

END

#endif // TimerManager_h__
