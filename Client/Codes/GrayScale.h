#ifndef GrayScale_h__
#define GrayScale_h__

#include "Effect.h"

class CGrayScale : public CEffect
{
protected:
	explicit CGrayScale();
	virtual ~CGrayScale();

public:
	DeclRTTI(CGrayScale, CEffect);

public:
	virtual HRESULT Init();
	virtual void	Render();

private:
	RCP_D3DTEXTURE	m_pRenderTarget;
	RCP_SURFACE		m_pDepthStencil;

public:
	CREATE(CGrayScale);
};

TYPEDEF_RCP(CGrayScale, GRAY);

#endif // GrayScale_h__
