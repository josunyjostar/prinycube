
#ifndef BubbleEX_h__
#define BubbleEX_h__

#include "AniEffect.h"

class CBubbleEX : public CAniEffect
{
private:
	explicit CBubbleEX(void);
	virtual ~CBubbleEX(void);
public:
	DeclRTTI(CBubbleEX, CAniEffect);
public:
	void	SetState(const _vec3& pStartPos,float fLifeTime = 60.f);
private:
	virtual HRESULT Init();
	virtual void	Update();
	virtual	void	Render();
public:
	CREATE(CBubbleEX);
};

TYPEDEF_RCP(CBubbleEX,BUBBLE_EX);

#endif // BubbleEX_h__
