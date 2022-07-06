#ifndef ItemHeart_h__
#define ItemHeart_h__

#include "AniEffect.h"

class CItemHeart : public CAniEffect
{
protected:
	explicit CItemHeart();
	virtual ~CItemHeart();

public:
	DeclRTTI(CItemHeart, CAniEffect);

	virtual HRESULT Init();

public:
	CREATE(CItemHeart);
};

TYPEDEF_RCP(CItemHeart, ITEMHEART);

#endif // ItemHeart_h__
