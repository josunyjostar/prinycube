#ifndef Stage09_h__
#define Stage09_h__

#include "StageScene.h"
#include "ColorCube.h"
#include "MirrorCube.h"

class CStage09 : public CStageScene
{
private:
	explicit CStage09();
	virtual ~CStage09();

public:
	DeclRTTI(CStage09, CStageScene);

	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();

private:
	RCP_COLORCUBE pColorCube1;
	RCP_COLORCUBE pColorCube2;
	RCP_COLORCUBE pColorCube3;
	RCP_COLORCUBE pColorCube4;
	RCP_MIRRORCUBE pMirrorCube1;
	RCP_MIRRORCUBE pMirrorCube2;

public:
	CREATE(CStage09);
};

TYPEDEF_RCP(CStage09, STAGE09);

#endif // Stage09_h__
