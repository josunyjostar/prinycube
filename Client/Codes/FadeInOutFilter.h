#ifndef FadeInOutFilter_h__
#define FadeInOutFilter_h__

#include "Include.h"

class CFadeOutFilter : public Engine::CGameObject
{
protected:
	explicit CFadeOutFilter();
	virtual ~CFadeOutFilter();

public:
	DeclRTTI(CFadeOutFilter, Engine::CGameObject);

	void SetColor(const _vec4& _vColor)				{ m_vColor = _vColor; }
	void SetTargetColor(const _vec4& _vTargetColor) { m_vTargetColor = _vTargetColor; }
	void SetPosition(_vec3 _vUIPos)					{ m_pTransform->SetPosition(UI_POS(_vUIPos.x, _vUIPos.y)); }
	void SetPosition(_vec2 _vUIPos)					{ m_pTransform->SetPosition(UI_POS(_vUIPos.x, _vUIPos.y)); }

public:
	virtual HRESULT Init();
	virtual void	Render();
	void			FadeIn(float _fFadeTime = 0.5f, FADE_FILTER_TYPE _eFadeType = FADE_NORMAL);
	void			FadeOut(float _fFadeTime = 0.5f, FADE_FILTER_TYPE _eFadeType = FADE_NORMAL);

private:
	RCP_DEVICE		m_pGraphicDev;
	RCP_TRANSFORM	m_pTransform;
	RCP_RESOURCE	m_pResource;
	RCP_SHADER		m_pShader;
	RCP_TEXTURE		m_pTexture;
	RCP_TIMER		m_pTimer;
	bool			m_bFadeIn;
	float			m_fFadeTime;
	float			m_fMaxFadeTime;
	_vec4			m_vColor;
	_vec4			m_vTargetColor;

	FADE_FILTER_TYPE m_eFadeType;

private:
	void			SetFadeFilter(float _fFadeTime, FADE_FILTER_TYPE _eFadeType);

public:
	CREATE(CFadeOutFilter);
};

TYPEDEF_RCP(CFadeOutFilter, FADEINOUTFILTER);

#endif // FadeInOutFilter_h__
