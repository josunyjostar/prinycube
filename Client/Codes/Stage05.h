#ifndef Stage05_h__
#define Stage05_h__

#include "StageScene.h"
#include "ColorCube.h"

class CStage05 : public CStageScene
{
private:
	explicit CStage05();
	virtual ~CStage05();

public:
	DeclRTTI(CStage05, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

private:
	RCP_COLORCUBE pColorCube1;

public:
	CREATE(CStage05);
};

TYPEDEF_RCP(CStage05, STAGE05);

#endif // Stage05_h__
