#ifndef Stage06_2_h__
#define Stage06_2_h__

#include "StageScene.h"

class CStage06_2 : public CStageScene
{
private:
	explicit CStage06_2();
	virtual ~CStage06_2();

public:
	DeclRTTI(CStage06_2, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage06_2);
};

TYPEDEF_RCP(CStage06_2, STAGE06_2);

#endif // Stage06_2_h__
