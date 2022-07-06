#include "stdafx.h"
#include "MainApp.h"
/*
#ifdef _DEBUG
#include "vld.h"
#endif
*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

RCP_MAINAPP		pMainApp;
bool			g_bIsEnable = false;
static int		g_wheel = 0;

Engine::KeyState* const Engine::KeyInput = &Engine::KeyState::Inst();
Engine::CRayCast* const Engine::RayCast = &Engine::CRayCast::Inst();
Engine::CCallbackManager* const Engine::CallBackMgr = &Engine::CCallbackManager::Inst();
Engine::CSoundManager* const Engine::SoundMgr = &Engine::CSoundManager::Inst();

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(n);
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LPCTSTR lpczTitle = L"Client";
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));;

	RECT rt = { 0, 0, WINCX, WINCY };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIconSm = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = lpczTitle;
	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(lpczTitle, lpczTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rt.right-rt.left, rt.bottom-rt.top, NULL, (HMENU)NULL, hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, L"Window Created Filed", L"Error Message", MB_ICONERROR | MB_OK);
		return -1;
	}
	Engine::KeyInput->Init(g_hWnd = hWnd);
	Engine::RayCast->Init(&Engine::CCollisionManager::Inst());
	ShowWindow(hWnd, nCmdShow);

	g_hInst = hInstance;

	MSG Msg;
	memset(&Msg, 0, sizeof(Msg));


	//Timer Init
	srand(unsigned(time(NULL)));

	Engine::CTimerManager* pTimerMgr = &Engine::CTimerManager::Inst();
	RCP_TIMER pFrame = pTimerMgr->CreateTimer("MainApp_Frame");
	if(!pFrame) return FALSE;

	pFrame->SetFrameDelay(1.f / MAX_FRAME);



	//Game Init
	pMainApp = CMainApp::Create();
	if(!pMainApp) return FALSE;


	//Process
	while (Msg.message != WM_QUIT)
	{
		if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else {
			pFrame->Update();
			if(pFrame->IsFrameReady())
			{
				if(g_bIsEnable) {
					Engine::KeyInput->Update(g_wheel);
					g_wheel = 0;

					pMainApp->Update();
				}
			}
		}
	}

	UnregisterClass(lpczTitle, wcex.hInstance);
	return (int) Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_SYSKEYDOWN:
		if(wParam == VK_MENU) return 0;

	case WM_ACTIVATEAPP :
		(wParam==1)?g_bIsEnable = true:g_bIsEnable = false;
		break;

	case WM_MOUSEWHEEL:
		g_wheel -= short(HIWORD(wParam)) / 120;
		break;

	case WM_SIZING:{
		RECT* r = (RECT*)lParam;

		//Win X tSize Limit
		//Win X MinSize
		if (r->right - r->left < R_WINCX + 16){
			if (wParam == WMSZ_LEFT || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_TOPLEFT) 
				r->left = r->right - R_WINCX - 16;
			if (wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT) 
				r->right = r->left + R_WINCX + 16;
		}
		//Win X MaxSize
		if (r->right - r->left > R_WINCX + 16){
			if (wParam == WMSZ_LEFT || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_TOPLEFT) 
				r->left = r->right - R_WINCX - 16;
			if (wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT) 
				r->right = r->left + R_WINCX + 16;
		}

		//Win Y tSize Limit
		//Win Y MinSize
		if (r->bottom - r->top < R_WINCY + 38){
			if (wParam == WMSZ_TOP || wParam == WMSZ_TOPRIGHT || wParam == WMSZ_TOPLEFT)
				r->top = r->bottom - R_WINCY - 38;
			if (wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_BOTTOMLEFT)
				r->bottom = r->top + R_WINCY + 38;
		}
		//Win Y MaxSize
		if (r->bottom - r->top > R_WINCY + 38){
			if (wParam == WMSZ_TOP || wParam == WMSZ_TOPRIGHT || wParam == WMSZ_TOPLEFT) 
				r->top = r->bottom - R_WINCY - 38;
			if (wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_BOTTOMLEFT) 
				r->bottom = r->top + R_WINCY + 38;
		}
		break;
	}

	case WM_PAINT:{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		pMainApp->Render();

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}