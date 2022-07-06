#ifndef LoadingPrinny_h__
#define LoadingPrinny_h__

#include "Include.h"
#include "Animator.h"

class CLoadingPrinny : public Engine::CGameObject
{
private:
	explicit CLoadingPrinny();
	virtual ~CLoadingPrinny();

public:
	DeclRTTI(CLoadingPrinny, Engine::CGameObject);

public:
	virtual HRESULT Init();
	virtual void	Render();
	void			Finish();

public:
	RCP_DEVICE		m_pDev;
	RCP_RC_TEX		m_pResource;
	RCP_ANIMATOR	m_pAnimator;
	RCP_TRANSFORM	m_pTransform;
	RCP_TIMER		m_pTimer;

public:
	CREATE(CLoadingPrinny);
};

TYPEDEF_RCP(CLoadingPrinny, LOADINGPRINNY);

#endif // LoadingPrinny_h__
