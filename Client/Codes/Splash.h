#ifndef Splash_h__
#define Splash_h__

#include "AniEffect.h"

class CPlayer;
class CSplash : public CAniEffect
{
private:
	explicit CSplash(void);
	virtual ~CSplash(void);
public:
	DeclRTTI(CSplash, CAniEffect);
public:
	void SetCreatePos(_vec3& pPos);
private:
	virtual HRESULT Init();
	virtual void Render();
public:
	CREATE(CSplash);
};

TYPEDEF_RCP(CSplash,SPLASH);

#endif // Splash_h__