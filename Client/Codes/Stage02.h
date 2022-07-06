#ifndef Stage02_h__
#define Stage02_h__

#include "StageScene.h"
#include "StageSwapCube.h"

class CStage02 : public CStageScene
{
private:
	explicit CStage02();
	virtual ~CStage02();

public:
	DeclRTTI(CStage02, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage02);
};

TYPEDEF_RCP(CStage02, STATGE02);

#endif // Stage02_h__