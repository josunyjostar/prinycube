#ifndef Stage07_1_h__
#define Stage07_1_h__

#include "StageScene.h"

class CStage07_1 : public CStageScene
{
private:
	explicit CStage07_1();
	virtual ~CStage07_1();

public:
	DeclRTTI(CStage07_1, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage07_1);
};

TYPEDEF_RCP(CStage07_1, STAGE07_1);

#endif // Stage07_1_h__
