#ifndef Button_h__
#define Button_h__

#include "UI.h"

class CButton : public CUI
{
protected:
	explicit CButton();
	virtual ~CButton();

public:
	DeclRTTI(CButton, CUI);

public:
	virtual void	Update();
	virtual void	Render();

	virtual void	OnMouseEnter()		{}
	virtual void	OnMouseExit()		{}
	virtual void	OnMouseButtonDown()	{}
	virtual void	OnMouseButtonUp()	{}

public:
	_vec3			m_vPrevScale;
	bool			m_bEnter;
	bool			m_bButtonDown;

public:
	CREATE(CButton);
};

TYPEDEF_RCP(CButton, BUTTON);

#endif // Button_h__
