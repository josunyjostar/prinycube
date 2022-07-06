#ifndef ObjectFont_h__
#define ObjectFont_h__

#include "Include.h"

class CObjectFont : public Engine::CGameObject
{
protected:
	explicit CObjectFont();
	virtual ~CObjectFont();

public:
	DeclRTTI(CObjectFont, Engine::CGameObject);

	void			SetString(const char* _pStr)		{ m_pFont->SetString(_pStr); }
	void			SetPosition(const _vec2& _vPos)		{ m_pFont->SetPosition(_vPos); }
	void			SetColor(const D3DCOLOR& _Color)	{ m_pFont->SetColor(_Color); }
	RCP_FONT		GetFont() const { return m_pFont; }

public:
	virtual HRESULT Init();
	virtual void	Render();

private:
	RCP_DEVICE		m_pGraphicDev;
	RCP_FONT		m_pFont;

public:
	CREATE(CObjectFont);
};

TYPEDEF_RCP(CObjectFont, OBJECTFONT);

#endif // ObjectFont_h__
