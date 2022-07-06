#include "stdafx.h"
#include "Define.h"

_matrix			g_matLT = _matrix( 0.707107f,0, 0.707107f,0, 0,1,0,0, -0.707107f,0, 0.707107f,0, 0,0,0,1);
_matrix			g_matLB = _matrix(-0.707107f,0, 0.707107f,0, 0,1,0,0, -0.707107f,0,-0.707107f,0, 0,0,0,1);
_matrix			g_matRB = _matrix(-0.707107f,0,-0.707107f,0, 0,1,0,0,  0.707107f,0,-0.707107f,0, 0,0,0,1);
_matrix			g_matRT = _matrix( 0.707107f,0,-0.707107f,0, 0,1,0,0,  0.707107f,0, 0.707107f,0, 0,0,0,1);
_matrix			g_matUIView = _matrix(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,1,1);
_matrix			g_matUIProj = _matrix(2,0,0,0, 0,2,0,0, 0,0,0.5f,0, 0,0,0,1);

HWND			g_hWnd;
HINSTANCE		g_hInst;
float			g_fTime;
float			g_fLightAmount = 1.f;

const int WINCX = 1280;
const int WINCY = 720;
int R_WINCX = WINCX;
int R_WINCY = WINCY;

GAME_DATA g_GameData;