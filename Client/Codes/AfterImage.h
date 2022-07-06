#ifndef AfterImage_h__
#define AfterImage_h__

#include "Effect.h"
#include "Player.h"

class CAfterIamge : public CEffect
{
private:
	explicit CAfterIamge(void);
	virtual ~CAfterIamge(void);
public:
	DeclRTTI(CAfterIamge,CEffect);
	void Setting(RCP_PLAYER _pPlayer);
public:
	CREATE(CAfterIamge);
};

TYPEDEF_RCP(CAfterIamge,CEffect);

#endif // AfterImage_h__
