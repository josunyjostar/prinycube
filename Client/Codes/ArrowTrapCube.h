#ifndef ArrowTrapCube_h__
#define ArrowTrapCube_h__

#include "BasicCube.h"
#include "StageScene.h"

class CArrowTrapCube : public CBasicCube
{
protected:
	explicit CArrowTrapCube();
	virtual ~CArrowTrapCube();
	DeclRTTI(CArrowTrapCube, CBasicCube);

public:
	virtual void	Update();

private:
	VIEW_DIR m_eDir;
	float m_fShotInterval;
	float m_fDeltaTime;
	float m_fTimer;

public:
	CREATE(CArrowTrapCube);
	void Setting(RCP_STAGESCENE _StageScene, _vec3 _StartPoint, VIEW_DIR _Dir, float _ShotInterval, RCP_SHADOWLIGHT _ShadowLight);
	RCP_SHADOWLIGHT m_ShadowLight;
};

TYPEDEF_RCP(CArrowTrapCube, ARROWTRAPCUBE);
#endif // ArrowTrapCube_h__
