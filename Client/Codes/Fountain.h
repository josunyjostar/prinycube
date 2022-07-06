
#ifndef Fountain_h__
#define Fountain_h__

#include "AniEffect.h"

class CFountain : public CAniEffect
{
private:
	explicit CFountain(void);
	virtual ~CFountain(void);
public:
	DeclRTTI(CFountain, CAniEffect);
private:
	virtual HRESULT Init();
	virtual void Render();

public:
	CREATE(CFountain);
};

TYPEDEF_RCP(CFountain,FOUNTAIN);

#endif // Fountain_h__