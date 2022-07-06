#ifndef PrismTri_Texture_h__
#define PrismTri_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class CPrismTri_Texture : public Engine::CVIBuffer
{
protected:
	explicit CPrismTri_Texture();
	virtual ~CPrismTri_Texture();

public:
	DeclRTTI(CPrismTri_Texture, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CPrismTri_Texture, PRISMTRI_TEX);

#endif // PrismTri_Texture_h__
