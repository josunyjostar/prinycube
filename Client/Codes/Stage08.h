#ifndef Stage08_h__
#define Stage08_h__

#include "StageScene.h"
#include "StageSwapCube.h"

class CStage08 : public CStageScene
{
private:
	explicit CStage08();
	virtual ~CStage08();

public:
	DeclRTTI(CStage08, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage08);
};

TYPEDEF_RCP(CStage08, STAGE08);

#endif // Stage08_h__