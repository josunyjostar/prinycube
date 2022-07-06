#ifndef Stage07_2_h__
#define Stage07_2_h__

#include "StageScene.h"

class CStage07_2 : public CStageScene
{
private:
	explicit CStage07_2();
	virtual ~CStage07_2();

public:
	DeclRTTI(CStage07_2, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage07_2);
};

TYPEDEF_RCP(CStage07_2, STAGE07_2);

#endif // Stage07_2_h__
