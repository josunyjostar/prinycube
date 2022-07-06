#ifndef EndingPrinny_h__
#define EndingPrinny_h__

#include "Include.h"
#include "Animator.h"

class CEndingPrinny : public Engine::CGameObject
{
private:
	explicit CEndingPrinny();
	virtual ~CEndingPrinny();

public:
	DeclRTTI(CEndingPrinny, Engine::CGameObject);

	int GetLife() const { return m_iLife; }

public:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();

	virtual void	CollisionEnter(Engine::COL_INFO& _ColInfo);

public:
	RCP_DEVICE		m_pDev;
	RCP_RC_TEX		m_pResource;
	RCP_ANIMATOR	m_pAnimator;
	RCP_TRANSFORM	m_pTransform;
	RCP_TIMER		m_pTimer;
	float			m_fDeltaTime;

	bool			m_bSceneChange;
	bool			m_bJump;
	float			m_fY;
	float			m_fJumpPower;
	float			m_fVelocity;

	int				m_iPos;
	int				m_iLife;
	bool			m_bDead;
	RCP_LAYER		m_pEffectLayer;

public:
	CREATE(CEndingPrinny);
};

TYPEDEF_RCP(CEndingPrinny, ENDINGPRINNY);

#endif // EndingPrinny_h__
