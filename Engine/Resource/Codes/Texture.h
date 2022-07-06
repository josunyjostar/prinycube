#ifndef Texture_h__
#define Texture_h__

#include "Resource.h"

BEGIN(Engine)

enum TEXTURE_TYPE { TEX_NORMAL, TEX_CUBE, TEX_END };

class ENGINE_DLL CTexture : public CResource
{
protected:
	explicit CTexture();
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	CTexture& operator=(const CTexture& rhs);

public:
	DeclRTTI(CTexture, CResource);
	friend CComponent;

	
	void SetTexture(const _ushort& iIndex = 0)	{ if(m_TextureSize > iIndex) m_pGraphicDev->SetTexture(0, m_ppTexture[iIndex]); }
	_ushort GetTextureSize() const				{ return m_TextureSize; }
	IDirect3DBaseTexture9* GetTexture(const _ushort& iIndex = 0)	{ return m_ppTexture[iIndex]; }

public:
	virtual void Render(RCP_RESOURCE _pResource, const _ushort& iIndex = 0);
	virtual HRESULT	CreateTexture(TEXTURE_TYPE _eType, const _char* _pFilePath, _ushort _wCnt = 1, _ushort _wStartNumber = 0);

protected:
	IDirect3DBaseTexture9**		m_ppTexture;
	_ushort						m_TextureSize;

};

END

TYPEDEF_RCP(Engine::CTexture, TEXTURE);

#endif // Texture_h__
