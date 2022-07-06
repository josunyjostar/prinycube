#ifndef DEFINE_H__
#define DEFINE_H__

#include "SmartPointer.h"
#include "Macro.h"

TYPEDEF_RCP(IDirect3D9,				SDK);
TYPEDEF_RCP(IDirect3DDevice9,		DEVICE);
TYPEDEF_RCP(IDirect3DVertexBuffer9, VB);
TYPEDEF_RCP(IDirect3DIndexBuffer9,	IB);
TYPEDEF_RCP(IDirect3DBaseTexture9,	BASETEXTURE);
TYPEDEF_RCP(IDirect3DTexture9,		D3DTEXTURE);
TYPEDEF_RCP(IDirect3DSurface9,		SURFACE);
TYPEDEF_RCP(ID3DXEffect,			SHADER);

#define MAX_FRAME			64
#define MAX_CAM_DIST		20.f
#define MIN_CAM_DIST		1.f
#define GRAVITY_VELOCITY	0.8f
#define JUMP_POWER			6.5f


enum RESOURCE_TYPE { RESOURCE_STATIC, RESOURCE_LOADING, RESOURCE_TITLE, RESOURCE_STAGE, RESOURCE_END };
enum WALL_DIR{ WALL_LEFT, WALL_RIGHT, WALL_END };
enum VIEW_DIR{ DIR_LT, DIR_LB, DIR_RB, DIR_RT, DIR_END };
enum FADE_FILTER_TYPE{ FADE_NORMAL, FADE_IN, FADE_OUT, FADE_END };


enum SOUND_TYPE
{
	SND_BUBBLE, SND_BUTTON_CLICK, SND_BUTTON_ENTER, SND_CLEAR, SND_DEAD, SND_DEAD_CLICK, SND_GET_HEART, SND_WHEEL,
	SND_DIVE, SND_GROUND, SND_JUMP_DOWN, SND_JUMP_UP, SND_LASER, SND_PAUSE, SND_ROTATION_ACTIVE, SND_ROTATION_SWING,
	SND_SWAP, SND_THROW, SND_TILE_CLICK, SND_WATER_CLICK, SND_SCENE_CHANGE, SND_ARROW_FIRE, SND_ARROW_HIT, SND_STONE_HIT,
	
	BGM_TITLE, BGM_GAME, BGM_ENDING, BGM_STAGE_06, SND_END
};


#define VEC_LB(__FLOAT)			(Normalize(_vec3(-1.f, 0.f, -1.f))*((float)__FLOAT))
#define VEC_LT(__FLOAT)			(Normalize(_vec3(-1.f, 0.f, 1.f))*((float)__FLOAT))
#define VEC_RB(__FLOAT)			(Normalize(_vec3(1.f, 0.f, -1.f))*((float)__FLOAT))
#define VEC_RT(__FLOAT)			(Normalize(_vec3(1.f, 0.f, 1.f))*((float)__FLOAT))
#define VEC_X(__FLOAT)			(_vec3(((float)__FLOAT), 0.f, 0.f))
#define VEC_Y(__FLOAT)			(_vec3(0.f, ((float)__FLOAT), 0.f))
#define VEC_Z(__FLOAT)			(_vec3(0.f, 0.f, ((float)__FLOAT)))
#define VEC3(__FX,__FY,__FZ)	(VEC_RB(__FX) + VEC_Y(__FY) + VEC_RT(__FZ))
#define VEC3EX(__FX,__FY,__FZ)	(VEC_RB(__FX)*1.1f + VEC_Y(__FY) + VEC_RT(__FZ)*1.1f)

#include "Engine_Math.h"
extern _matrix			g_matLT;
extern _matrix			g_matLB;
extern _matrix			g_matRB;
extern _matrix			g_matRT;
extern _matrix			g_matUIView;
extern _matrix			g_matUIProj;

extern HWND				g_hWnd;
extern HINSTANCE		g_hInst;
extern float			g_fTime;
extern float			g_fLightAmount;

extern const int		WINCX;
extern const int		WINCY;
extern int				R_WINCX;
extern int				R_WINCY;


#define UI_CX(__CX)				((__CX) / (float)WINCX)
#define UI_CY(__CY)				((__CY) / (float)WINCY)
#define UI_SIZE(__CX,__CY)		(_vec3(UI_CX(__CX), UI_CY(__CY), 0.f))
#define UI_X(__X)				((__X) / (float)WINCX - 0.5f)
#define UI_Y(__Y)				(0.5f - (__Y) / (float)WINCY)
#define UI_POS(__X,__Y)			(_vec3(UI_X(__X), UI_Y(__Y), 0.f))


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


#define MAX_STAGE	10
typedef struct tagGameData {
	tagGameData() :ClearStage(-1) { memset(MinDeadCount, 0, sizeof(int)*MAX_STAGE); memset(DeadCount, 0, sizeof(int)*MAX_STAGE); }
	void SetRunningStage(int _iStage) { RunningStage = _iStage; DeadCount[_iStage] = 0; }

	inline void Load();
	inline void Save();

	int RunningStage;
	int DeadCount[MAX_STAGE];
	int ClearStage;
	int MinDeadCount[MAX_STAGE];
}GAME_DATA;

extern GAME_DATA g_GameData;

void tagGameData::Load()
{
	FILE* pFile;
	if(fopen_s(&pFile, "../Resources/GameData/Save.dat", "a+b") != NULL)
	{
		ERR_MSG("File Open Failed!");
		return;
	}

	fread(&ClearStage, sizeof(int), 1, pFile);
	fread(MinDeadCount, sizeof(int)*MAX_STAGE, 1, pFile);

	fclose(pFile);
}

void tagGameData::Save()
{
	FILE* pFile;
	if(fopen_s(&pFile, "../Resources/GameData/Save.dat", "wb") != NULL)
	{
		ERR_MSG("File Open Failed!");
		return;
	}

	if(ClearStage < RunningStage) {
		ClearStage = RunningStage;
		MinDeadCount[RunningStage] = DeadCount[RunningStage];
	}
	else {
		if(MinDeadCount[RunningStage] > DeadCount[RunningStage]) MinDeadCount[RunningStage] = DeadCount[RunningStage];
	}

	fwrite(&ClearStage, sizeof(int), 1, pFile);
	fwrite(MinDeadCount, sizeof(int)*MAX_STAGE, 1, pFile);

	fclose(pFile);
}


#endif //DEFINE_H__