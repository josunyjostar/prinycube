#ifndef FontManager_h__
#define FontManager_h__

#include "Font.h"
#include "TemplateSingleton.h"
#include "TinyHashTable.h"

BEGIN(Engine)

class ENGINE_DLL CFontManager : public TSingleton<CFontManager>
{
private:
	explicit CFontManager();
	virtual ~CFontManager();

public:
	friend TSingleton;

	void		Init(RCP_DEVICE& _pDev)				{ m_pDev = _pDev; }
	RCP_FONT	CreateFont(const char* _pFontStyle, _int _iHeight, _uint _iWidth, _uint _iWeight);
	RCP_FONT&	FindFont(const char* _pFontStyle)	{ return m_hFonts[_pFontStyle]; }

private:
	TinyHashTable<const _char*,RCP_FONT>	m_hFonts;
	RCP_DEVICE								m_pDev;

public:
	void Release();
};

END

#endif // FontManager_h__
