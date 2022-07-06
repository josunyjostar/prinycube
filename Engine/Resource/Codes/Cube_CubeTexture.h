#ifndef Cube_CubeTexture_h__
#define Cube_CubeTexture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube_CubeTexture : public CVIBuffer
{
protected:
	explicit CCube_CubeTexture();
	virtual ~CCube_CubeTexture();

public:
	DeclRTTI(CCube_CubeTexture, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CCube_CubeTexture, CUBE_CUBETEX);

#endif // Cube_CubeTexture_h__
