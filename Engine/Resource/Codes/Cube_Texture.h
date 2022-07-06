#ifndef Cube_Texture_h__
#define Cube_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube_Texture : public CVIBuffer
{
protected:
	explicit CCube_Texture();
	virtual ~CCube_Texture();

public:
	DeclRTTI(CCube_Texture, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CCube_Texture, CUBE_TEX);

#endif // Cube_Texture_h__
