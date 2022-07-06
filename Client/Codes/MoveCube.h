#ifndef MoveTile_h__
#define MoveTile_h__

#include "BasicCube.h"

class CMoveCube : public CBasicCube
{
protected:
	explicit CMoveCube();
	virtual ~CMoveCube();

public:
	DeclRTTI(CMoveCube, CBasicCube);

	void Setting(RCP_STAGESCENE _StageScene, _vec3 _StartPoint, _vec3 _EndPoint, float _TimeInterval, float _StopInterval, RCP_SHADOWLIGHT _ShadowLight);
	_vec3 GetMotionVector() const;

public:
	virtual void Update();	

protected:
	_vec3		m_vStartPoint;
	_vec3		m_vEndPoint;
	_vec3		m_PrevPoint;

	float		m_fTimeInterval;
	float		m_fStopInterval;
	float		m_fDeltaTime;
	float		m_fStopTimer;
	float		m_fTimer;
	bool		m_bReversal;

public:
	CREATE(CMoveCube);
};

TYPEDEF_RCP(CMoveCube, MOVECUBE);

#endif // MoveTile_h__
