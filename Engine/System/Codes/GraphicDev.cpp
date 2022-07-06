#include "GraphicDev.h"
using namespace Engine;

CGraphicDev::CGraphicDev()
{
}

CGraphicDev::~CGraphicDev()
{
	this->Release();
}

HRESULT CGraphicDev::Init(HWND _hWnd, bool _bWindowed, int _iWinWidth, int _iWinHeight)
{
	if(m_pSDK) return S_OK;

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_pSDK) 
	{
		ERR_MSG("Direct3D Created Failed");
		return E_FAIL;
	}
	m_pSDK->Release();

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth		= _iWinWidth;
	d3dpp.BackBufferHeight		= _iWinHeight;
	d3dpp.BackBufferFormat		= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount		= 1;
	d3dpp.MultiSampleType		= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality	= 0;
	d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow			= _hWnd;
	d3dpp.Windowed				= _bWindowed;
	d3dpp.EnableAutoDepthStencil= true;
	d3dpp.AutoDepthStencilFormat= D3DFMT_D24S8;
	d3dpp.Flags					= 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;
	m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	vp |= D3DCREATE_MULTITHREADED;

	LPDIRECT3DDEVICE9 pd3dDevice;
	if(FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, _hWnd, vp, 
		&d3dpp, &pd3dDevice)))
	{
		ERR_MSG("Device Created Failed");
		return E_FAIL;
	}
	m_pd3dDevice = pd3dDevice;
	pd3dDevice->Release();

	return S_OK;
}

void CGraphicDev::Release()
{
	DWORD dw;
	dw = m_pd3dDevice.Release();
	dw = m_pSDK.Release();
	if(dw) ERR_MSG("Device Release Failed");
}