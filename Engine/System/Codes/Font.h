#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont : public CBase
{
protected:
	explicit CFont();
	virtual ~CFont();

public:
	DeclRTTI(CFont, CBase);

	void		SetString(const char* _pStr)		{ m_pStr = _pStr; }
	void		SetPosition(const _vec2& _vPos)		{ m_vPosition = _vPos; }
	void		SetColor(const D3DCOLOR& _Color)	{ m_Color = _Color; }
	void		SetFont(const char* _pStr, const _vec2& _vPos, const D3DCOLOR& _Color = D3DCOLOR_XRGB(255, 255, 255)) 
													{ m_pStr = _pStr; m_vPosition = _vPos; m_Color = _Color; }

	const char* GetString() const					{ return m_pStr; }
	_vec2		GetPosition() const					{ return m_vPosition; }
	D3DCOLOR	GetColor() const					{ return m_Color; }

public:
	HRESULT		InitFont(RCP_DEVICE _pGraphicDev, const char* _pFontStyle, _int _iHeight, _uint _iWidth, _uint _iWeight);
	void		Render();

private:
	LPD3DXFONT	m_pFont;
	const char*	m_pStr;
	_vec2		m_vPosition;
	D3DCOLOR	m_Color;

public:
	CREATE(CFont);
};

END

TYPEDEF_RCP(Engine::CFont, FONT);

#endif // Font_h__
