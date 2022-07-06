#ifndef Stage03_h__
#define Stage03_h__
#include "StageScene.h"
#include "StageSwapCube.h"

class CStage03 : public CStageScene
{
private:
	explicit CStage03();
	virtual ~CStage03();

public:
	DeclRTTI(CStage03, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

public:
	CREATE(CStage03);
};

TYPEDEF_RCP(CStage03, STATGE03);

#endif // Stage03_h__