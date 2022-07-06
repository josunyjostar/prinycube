#include "Font.h"
#include <atlstr.h>
using namespace Engine;

CFont::CFont()
:m_pFont(NULL)
,m_pStr(NULL)
,m_Color(0)
{
}

CFont::~CFont()
{
	Safe_Release(m_pFont);
}

HRESULT CFont::InitFont(RCP_DEVICE _pGraphicDev, const char* _pFontStyle, _int _iHeight, _uint _iWidth, _uint _iWeight)
{
	if(m_pFont) return E_FAIL;

	D3DXFONT_DESCW FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESCW));

	FontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(FontInfo.FaceName, CA2CT(_pFontStyle));
	FontInfo.Height = _iHeight;
	FontInfo.Width  = _iWidth;
	FontInfo.Weight = _iWeight;

	if(FAILED(D3DXCreateFontIndirect(&*_pGraphicDev, &FontInfo, &m_pFont)))
	{
		ERR_MSG("Font Created Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render()
{
	RECT rc = {LONG(m_vPosition.x), LONG(m_vPosition.y)};
	m_pFont->DrawTextA(NULL, m_pStr, strlen(m_pStr), &rc, DT_NOCLIP, m_Color);
}