#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerManager.h"
#include "FontManager.h"

BEGIN(Engine)

inline void System_Release();

#include "Export_System.inl"

END

#endif // Export_System_h__
