#ifndef Bubble_h__
#define Bubble_h__

#include "AniEffect.h"

class CBubble : public CAniEffect
{
private:
	explicit CBubble(void);
	virtual ~CBubble(void);
public:
	DeclRTTI(CBubble, CAniEffect);
public:
	void	SetState(const _vec3& pStartPos,float fLifeTime = 60.f);
private:
	virtual HRESULT Init();
	virtual void	Update();
	virtual	void	Render();
public:
	CREATE(CBubble);
};

TYPEDEF_RCP(CBubble,BUBBLE);

#endif // Bubble_h__
