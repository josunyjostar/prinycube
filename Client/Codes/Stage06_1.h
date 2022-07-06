#ifndef Stage06_1_h__
#define Stage06_1_h__

#include "StageScene.h"
#include "StageSwapCube.h"

class CStage06_1 : public CStageScene
{
private:
	explicit CStage06_1();
	virtual ~CStage06_1();

public:
	DeclRTTI(CStage06_1, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

private:
	RCP_STAGESWAPCUBE m_pStageSwapCube1;
	RCP_STAGESWAPCUBE m_pStageSwapCube2;
	RCP_STAGESWAPCUBE m_pStageSwapCube3;
	RCP_STAGESWAPCUBE m_pStageSwapCube4;

public:
	CREATE(CStage06_1);
};

TYPEDEF_RCP(CStage06_1, STAGE06_1);

#endif // Stage06_1_h__
