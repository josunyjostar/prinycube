#ifndef DeadWater_h__
#define DeadWater_h__

#include "AniEffect.h"

class CPlayer;
class CDeadWater : public CAniEffect
{
private:
	explicit CDeadWater(void);
	virtual ~CDeadWater(void);
public:
	DeclRTTI(CDeadWater, CAniEffect);
public:
	void SetCreatePos(_vec3& pPos);
private:
	virtual HRESULT Init();
public:
	CREATE(CDeadWater);
};

TYPEDEF_RCP(CDeadWater,DEADWATER);

#endif // DeadWater_h__