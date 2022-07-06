#ifndef ShadowLight_h__
#define ShadowLight_h__

#include "Include.h"

#define		SHADOW_ACCEL	0.07f

class CShadowLight : public Engine::CGameObject
{
protected:
	explicit CShadowLight();
	virtual ~CShadowLight();

public:
	DeclRTTI(CShadowLight, Engine::CGameObject);

	void SetLightDir(bool _bLeft)	{ if(_bLeft != m_bLeft) { m_bLeft = _bLeft; m_fAccel = SHADOW_ACCEL; } }
	_matrix GetLightView() const	{ return m_matLightView; }

public:
	virtual HRESULT Init();
	virtual void	Update();

private:
	RCP_TIMER		m_pTimer;
	_matrix			m_matLeftLight;
	_matrix			m_matRightLight;
	_matrix			m_matLookLight;
	_matrix			m_matLightView;

	bool			m_bLeft;
	float			m_fTimeVal;
	float			m_fAccel;

public:
	CREATE(CShadowLight);
};

TYPEDEF_RCP(CShadowLight, SHADOWLIGHT);

#endif // ShadowLight_h__
