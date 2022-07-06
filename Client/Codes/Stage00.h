#ifndef Stage00_h__
#define Stage00_h__
#include "StageScene.h"
#include "StageSwapCube.h"

class CStage00 : public CStageScene
{
private:
	explicit CStage00();
	virtual ~CStage00();

public:
	DeclRTTI(CStage00, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage00);
};

TYPEDEF_RCP(CStage00, STAGE00);

#endif // Stage00_h__