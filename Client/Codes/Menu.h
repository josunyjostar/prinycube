#ifndef Menu_h__
#define Menu_h__

#include "UI.h"
#include "Button.h"

class CMenu : public CUI
{
protected:
	explicit CMenu();
	virtual ~CMenu();

public:
	DeclRTTI(CMenu, CUI);

	void SetMenuButton(RCP_BUTTON _pButton) { m_pMenuButton = _pButton; }

public:
	virtual HRESULT Init();

private:
	RCP_UI	m_pMenuButton;

public:
	CREATE(CMenu);
};

TYPEDEF_RCP(CMenu, MENU);

#endif // Menu_h__
