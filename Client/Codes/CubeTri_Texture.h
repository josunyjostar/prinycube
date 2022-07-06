#ifndef CubeTri_Texture_h__
#define CubeTri_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class CCubeTri_Texture : public Engine::CVIBuffer
{
protected:
	explicit CCubeTri_Texture();
	virtual ~CCubeTri_Texture();

public:
	DeclRTTI(CCubeTri_Texture, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CCubeTri_Texture, CUBETRI_TEX);

#endif // CubeTri_Texture_h__
