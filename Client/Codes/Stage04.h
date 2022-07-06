#ifndef Stage04_h__
#define Stage04_h__

#include "StageScene.h"

class CStage04 : public CStageScene
{
private:
	explicit CStage04();
	virtual ~CStage04();

public:
	DeclRTTI(CStage04, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage04);
};

TYPEDEF_RCP(CStage04, STAGE04);

#endif // Stage04_h__
