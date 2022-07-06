#ifndef TitleScene_h__
#define TitleScene_h__

#include "Include.h"

class CTitleScene : public Engine::CScene
{
private:
	explicit CTitleScene();
	virtual ~CTitleScene();

public:
	DeclRTTI(CTitleScene, Engine::CScene);

	virtual void	Enable();
	virtual void	Disable();
private:
	RCP_TIMER			m_pTimer;
	float				m_fDeltaTime;
private:
	void MakeFish(void* pData);
	void MakeJellyFish(void* pData);
private:
	virtual void Update();
private:
	RCP_FADEINOUTFILTER m_pFadeInOutFilter;

public:
	CREATE(CTitleScene);
};

TYPEDEF_RCP(CTitleScene, TITLESCENE);

#endif // TitleScene_h__
