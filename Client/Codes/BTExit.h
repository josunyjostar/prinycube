#ifndef BTExit_h__
#define BTExit_h__

#include "Button.h"

class CBTExit : public CButton
{
protected:
	explicit CBTExit() {}
	virtual ~CBTExit() {}

public:
	DeclRTTI(CBTExit, CButton);

	virtual void	OnMouseButtonUp()	{ 
		SendMessage(g_hWnd, WM_CLOSE, 0, 0);
	}

public:
	CREATE(CBTExit);
};

TYPEDEF_RCP(CBTExit, BTEXIT);

#endif // BTExit_h__
