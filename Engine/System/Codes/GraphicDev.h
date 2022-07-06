#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Include.h"
#include "TemplateSingleton.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public TSingleton<CGraphicDev>
{
private:
	CGraphicDev();
	~CGraphicDev();

public:
	friend TSingleton;

	RCP_SDK		GetSDK()	{ return m_pSDK; }
	RCP_DEVICE	GetDev()	{ return m_pd3dDevice; }

public:
	HRESULT		Init(HWND _hWnd, bool _bWindowed, int _iWinWidth, int _iWinHeight);

private:
	RCP_SDK		m_pSDK;
	RCP_DEVICE	m_pd3dDevice;

public:
	void		Release();
};

END

#endif // GraphicDev_h__
