#ifndef Stage01_h__
#define Stage01_h__

#include "StageScene.h"
#include "StageSwapCube.h"

class CStage01 : public CStageScene
{
private:
	explicit CStage01();
	virtual ~CStage01();

public:
	DeclRTTI(CStage01, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage01);
};

TYPEDEF_RCP(CStage01, STATGE01);

#endif // Stage01_h__