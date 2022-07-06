#ifndef GetHeart_h__
#define GetHeart_h__

#include "AniEffect.h"

class CGetHeart : public CAniEffect
{
protected:
	explicit CGetHeart();
	virtual ~CGetHeart();

public:
	DeclRTTI(CGetHeart, CAniEffect);

	virtual HRESULT Init();

public:
	CREATE(CGetHeart);
};

TYPEDEF_RCP(CGetHeart, GETHEART);

#endif // GetHeart_h__
