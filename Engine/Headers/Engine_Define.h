#ifndef Engine_DEFINE_H__
#define Engine_DEFINE_H__

#include "SmartPointer.h"

TYPEDEF_RCP(IDirect3D9,				SDK);
TYPEDEF_RCP(IDirect3DDevice9,		DEVICE);
TYPEDEF_RCP(IDirect3DVertexBuffer9, VB);
TYPEDEF_RCP(IDirect3DIndexBuffer9,	IB);
TYPEDEF_RCP(IDirect3DBaseTexture9,	BASETEXTURE);
TYPEDEF_RCP(IDirect3DTexture9,		D3DTEXTURE);
TYPEDEF_RCP(IDirect3DSurface9,		SURFACE);
TYPEDEF_RCP(ID3DXEffect,			SHADER);

namespace Engine {

	typedef signed char		_byte;
	typedef unsigned char	_ubyte;

	typedef char			_char;
	typedef wchar_t			_tchar;

	typedef signed short	_short;
	typedef unsigned short	_ushort;

	typedef signed int		_int;
	typedef unsigned int	_uint;

	typedef signed long		_long;
	typedef unsigned long	_ulong;

	typedef float			_float;
	typedef double			_double;

	typedef bool			_bool;

}

#endif //Engine_DEFINE_H__