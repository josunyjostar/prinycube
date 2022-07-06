#include "FontManager.h"
using namespace Engine;

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	this->Release();
}

RCP_FONT CFontManager::CreateFont(const char* _pFontStyle, _int _iHeight, _uint _iWidth, _uint _iWeight)
{
	RCP_FONT& pFont = m_hFonts[_pFontStyle];

	if(!pFont) {
		pFont = CFont::Create();
		if(FAILED(pFont->InitFont(m_pDev, _pFontStyle, _iHeight, _iWidth, _iWeight)))
		{
			pFont = NULL;
			return NULL;
		}
	}

	return pFont;
}

void CFontManager::Release()
{
	m_hFonts.Clear(true);
	m_pDev = NULL;
}