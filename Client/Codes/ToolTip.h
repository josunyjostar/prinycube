#ifndef ToolTip_h__
#define ToolTip_h__

#include "UI.h"

class CToolTip : public CUI
{
protected:
	explicit CToolTip();
	virtual ~CToolTip();

public:
	DeclRTTI(CToolTip, CUI);

	virtual void	Update();
	virtual void	Render();

	void			PopUp(float _fLifeTime = 1.f, float _fUpTime = 0.2f, float _fDownTime = 0.2f);

private:
	RCP_TIMER		m_pTimer;
	float			m_fDeltaTime;

	_vec3			m_vScale;
	float			m_fMaxLifeTime;
	float			m_fLifeTime;
	_float			m_fUpTime;
	_float			m_fDownTime;

public:
	CREATE(CToolTip);
};

TYPEDEF_RCP(CToolTip, TOOLTIP);

#endif // ToolTip_h__
