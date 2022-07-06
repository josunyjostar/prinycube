#ifndef JellyFish_h__
#define JellyFish_h__

#include "AniEffect.h"

class CJellyFish : public CAniEffect
{
private:
	explicit CJellyFish(void);
	virtual ~CJellyFish(void);
public:
	DeclRTTI(CJellyFish, CAniEffect);
public:
	void	SetState(const _vec3& pStartPos,float fLifeTime = 60.f);
private:
	virtual HRESULT Init();
	virtual void	Update();
	virtual	void	Render();
public:
	CREATE(CJellyFish);
};

TYPEDEF_RCP(CJellyFish,JELLYFISH);

#endif // JellyFish_h__
