#pragma once

#ifndef UNICODE
#error "DXUT requires a Unicode build."
#endif

#include "dxsdkver.h"
#if ( _DXSDK_PRODUCT_MAJOR < 9 || _DXSDK_BUILD_MAJOR < 1949 )
#error The installed DXSDK is out of date.
#endif

#ifndef STRICT
#define STRICT
#endif

// If app hasn't choosen, set to work with Windows XP and beyond
#ifndef WINVER
#define WINVER         0x0501
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600
#endif

#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3d10.lib" )
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx9d.lib" )
#pragma comment( lib, "d3dx10d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3dx10.lib" )
#endif
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "comctl32.lib" )

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

// Standard Windows includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>
#include <math.h>      
#include <limits.h>      
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <map>
#include <list>
#include <vector>
#include <string>

#include <typeinfo>
#include <ctime>
#include <stdarg.h>
typedef char* va_list;

#if defined(DEBUG) || defined(_DEBUG)
#include "Xtrace.h"
#endif

// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

// Direct3D10 includes
#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3dcompiler.h>
#include <d3dx10.h>

// XInput includes
#include <xinput.h>

// HRESULT translation for Direct3D and other APIs 
#include <dxerr.h>

using namespace std;


//TODO: