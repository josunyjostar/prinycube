#ifndef ActionCamera_h__
#define ActionCamera_h__

#include "Include.h"

class CActionCamera : public Engine::CCamera
{
private:
	explicit CActionCamera();
	virtual ~CActionCamera();

public:
	DeclRTTI(CActionCamera, Engine::CCamera);

	void SetTargetTransform(RCP_TRANSFORM _pTargetTransform) { m_pTargetTransform = _pTargetTransform; }

public:
	virtual void Update(const _float& _fDeltaTime);

private:
	RCP_TRANSFORM	m_pTargetTransform;
	float			m_fCamDist;
	float			m_fLerpDist;

public:
	CREATE(CActionCamera);
};

TYPEDEF_RCP(CActionCamera, ACTIONCAMERA);

#endif // ActionCamera_h__
