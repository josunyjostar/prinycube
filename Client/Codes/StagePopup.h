#ifndef StagePopup_h__
#define StagePopup_h__

#include "UI.h"

class CStagePopup : public CUI
{
protected:
	explicit CStagePopup();
	virtual ~CStagePopup();

public:
	DeclRTTI(CStagePopup, CUI);

	virtual HRESULT Init();
	virtual void	Update();

private:
	RCP_TIMER		m_pTimer;
	float			m_fDeltaTime;

	_vec3			m_vScale;
	float			m_fMaxLifeTime;
	float			m_fLifeTime;
	_float			m_fUpTime;
	_float			m_fDownTime;

public:
	CREATE(CStagePopup);
};

TYPEDEF_RCP(CStagePopup, STAGEPOPUP);

#endif // StagePopup_h__
