#ifndef Animator_h__
#define Animator_h__

#include "Include.h"

class CAnimator : public Engine::CComponent
{
public:
	explicit CAnimator(void);
	virtual ~CAnimator(void);

public:
	DeclRTTI(CAnimator, Engine::CComponent);

	void SetFrame(int _iFrame)			{m_fFrame = (float)_iFrame;}
	void SetPlaySpeed(float _fPlaySpeed){m_fPlaySpeed = _fPlaySpeed;}
	void SetResource(RCP_RESOURCE& _pResource) { m_pResource = _pResource; }

	bool IsPlay() const					{return m_bPlaying;}
	int GetFrame() const				{return (int)m_fFrame;}
	const char* GetPlayAniName() const	{return m_pPlayAniName;}

public:
	void Render(const _float& _fDeltaTime, bool _bUseShader = false);

	RCP_TEXTURE Play(int _iContainerIdx = -1, const _char* _pResourceTag = NULL, bool _bRepeatPlay = true, bool _bInit = true);
	void Play(const _char* _pPlayAniName = NULL, int _MaxFrame = -1, bool _bRepeatPlay = true, bool _bInit = true);
	void Stop(bool _bClear = false);

private:
	RCP_RESOURCE	m_pResource;
	RCP_TEXTURE		m_pPlayAni;
	const char*		m_pPlayAniName;
	int				m_iMaxFrame;
	float			m_fFrame;
	float			m_fPlaySpeed;
	bool			m_bPlaying;
	bool			m_bRepeat;

public:
	CREATE(CAnimator);
};

TYPEDEF_RCP(CAnimator, ANIMATOR);

#endif // Animator_h__
