#include "stdafx.h"
#include "Animator.h"

CAnimator::CAnimator(void)
:m_pPlayAniName(NULL)
,m_iMaxFrame(0)
,m_bRepeat(true)
,m_bPlaying(false)
,m_fPlaySpeed(1.f)
,m_fFrame(0.f)
{
}

CAnimator::~CAnimator(void)
{
}

void CAnimator::Render(const _float& _fDeltaTime, bool _bUseShader /*= false*/)
{
	if(m_bPlaying) {
		m_fFrame += _fDeltaTime*m_fPlaySpeed*m_iMaxFrame;
		if(m_fFrame >= m_iMaxFrame) {
			if(m_bRepeat) m_fFrame = 0.f;
			else {
				m_fFrame = m_iMaxFrame - 1.f;
				m_bPlaying = false;
			}
		}
	}

	if(!m_pResource) return;

	if(!m_pPlayAni || _bUseShader)
		m_pResource->Render();
	else
		m_pPlayAni->Render(m_pResource, (_ushort)m_fFrame);
}

RCP_TEXTURE CAnimator::Play(int _iContainerIdx /*= -1*/, const _char* _pResourceTag /*= NULL*/, bool _bRepeatPlay /*= true*/, bool _bInit /*= true*/)
{
	m_bPlaying		= true;

	if(_iContainerIdx == -1 || !_pResourceTag) return NULL;
	if(!_bInit && (m_pPlayAniName == _pResourceTag)) return Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(_iContainerIdx, _pResourceTag);

	RCP_TEXTURE pAni = Engine::CResourceManager::Inst().FindResource<Engine::CTexture>(_iContainerIdx, _pResourceTag);
	if(!pAni) return NULL;

	m_iMaxFrame		= pAni->GetTextureSize();
	m_fFrame		= 0.f;
	m_pPlayAni		= pAni;
	m_pPlayAniName	= _pResourceTag;
	m_bRepeat		= _bRepeatPlay;

	return pAni;
}

void CAnimator::Play(const _char* _pPlayAniName /*= NULL*/, int _MaxFrame /*= -1*/, bool _bRepeatPlay /*= true*/, bool _bInit /*= true*/)
{
	m_bPlaying		= true;

	if(_MaxFrame == -1 || !_pPlayAniName) return;
	if(!_bInit && (m_pPlayAniName == _pPlayAniName)) return;

	m_iMaxFrame		= _MaxFrame;
	m_fFrame		= 0.f;
	m_pPlayAni		= NULL;
	m_pPlayAniName	= _pPlayAniName;
	m_bRepeat		= _bRepeatPlay;
}

void CAnimator::Stop(bool _bClear /*= false*/)
{
	if(_bClear) Enable();
	else m_bPlaying = false;
}