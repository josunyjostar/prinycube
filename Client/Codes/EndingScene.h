#ifndef Ending_h__
#define Ending_h__

#include "Include.h"
#include "EndingPrinny.h"
#include "Life.h"

class CEndingScene : public Engine::CScene
{
protected:
	explicit CEndingScene();
	virtual ~CEndingScene();

public:
	DeclRTTI(CEndingScene, Engine::CScene);

	virtual void	Enable();
	virtual void	Update();

protected:
	RCP_DEVICE			m_pDev;
	RCP_TIMER			m_pTimer;
	RCP_FONT			m_pFont;
	RCP_ENDINGPRINNY	m_pPrinny;
	float				m_fDeltaTime;
	float				m_fPressKey;
	RCP_LIFE			m_pLife[3];
	int					m_iPrevLife;

public:
	CREATE(CEndingScene);
};

TYPEDEF_RCP(CEndingScene, ENDINGSCENE);

#endif // Ending_h__
