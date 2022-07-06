#ifndef Engine_Macro_h__
#define Engine_Macro_h__

#define SAFE_DELETE(p)			if(p){delete p;p=NULL;}
#define SAFE_DELETE_ARRAY(p)	if(p){delete[] p;p=NULL;}
#define SAFE_RELEASE(p)			if(p){p->Release();p=NULL;}

#if defined(SYSTEM_EXPORTS) || defined(UTILITY_EXPORTS) || defined(RESOURCE_EXPORTS)
#define ENGINE_DLL __declspec(dllexport)
#define ENGINE_DLL_TEMPLATE
#else
#define ENGINE_DLL __declspec(dllimport)
#define ENGINE_DLL_TEMPLATE extern
#endif

#define ERR_MSG(Message)		MessageBox(NULL, TEXT(Message), TEXT("Error"), MB_OK)

#define BEGIN(NAMESPACE)		namespace NAMESPACE {
#define END						}

#pragma warning(disable : 4251)

#endif // Engine_Macro_h__
