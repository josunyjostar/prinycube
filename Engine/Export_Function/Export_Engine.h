#ifndef Export_Engine_h__
#define Export_Engine_h__

#include "Export_System.h"
#include "Export_Resource.h"
#include "Export_Utility.h"

BEGIN(Engine)

inline void Manager_Init(RCP_DEVICE& _pDev, const _ushort& _wResourceSize, const _ushort& _wRendererSize)
{
	CFontManager::Inst().Init(_pDev);
	CShaderManager::Inst().Init(_pDev);
	CResourceManager::Inst().Init(_pDev, _wResourceSize);
	CLightManager::Inst().Init(_pDev);
	CCameraManager::Inst().Init(_pDev);
	CRenderManager::Inst().Init(_pDev, _wRendererSize);
}

inline void Manager_Release()
{
	Utility_Release();
	Resource_Release();
	System_Release();
}

END

#endif // Export_Engine_h__
