#ifndef StageSelectScene_h__
#define StageSelectScene_h__

#include "Include.h"

class CStageSelectScene : public Engine::CScene
{
private:
	explicit CStageSelectScene();
	virtual ~CStageSelectScene();

public:
	DeclRTTI(CStageSelectScene, Engine::CScene);

	virtual void	Enable();
	virtual void	Disable();

private:
	RCP_FADEINOUTFILTER m_pFadeInOutFilter;

public:
	CREATE(CStageSelectScene);
};

TYPEDEF_RCP(CStageSelectScene, STAGESELECTSCENE);


#endif // StageSelectScene_h__
