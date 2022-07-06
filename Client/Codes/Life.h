#ifndef Life_h__
#define Life_h__

#include "AniEffect.h"

class CLife : public CAniEffect
{
protected:
	explicit CLife();
	virtual ~CLife();

public:
	DeclRTTI(CLife, CAniEffect);

	virtual HRESULT Init();
	virtual void	Render();

public:
	CREATE(CLife);
};

TYPEDEF_RCP(CLife, LIFE);

#endif // Life_h__
