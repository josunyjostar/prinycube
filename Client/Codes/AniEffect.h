#ifndef AniEffect_h__
#define AniEffect_h__

#include "Effect.h"
#include "Animator.h"

class CAniEffect : public CEffect
{
protected:
	explicit CAniEffect();
	virtual ~CAniEffect();

public:
	DeclRTTI(CAniEffect, CEffect);

	virtual void	SetResource(RCP_RESOURCE _pResource);

public:
	void			Play(int _iContainerIdx, const _char* _pResourceTag, bool _bRepeatPlay = true);
	virtual void	Render();

protected:
	RCP_ANIMATOR	m_pAnimator;

public:
	CREATE(CAniEffect);
};

TYPEDEF_RCP(CAniEffect, ANIEFFECT);

#endif // AniEffect_h__
