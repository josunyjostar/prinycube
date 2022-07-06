#ifndef CubeTemple_Texture_h__
#define CubeTemple_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class CCubeTemple_Texture : public Engine::CVIBuffer
{
protected:
	explicit CCubeTemple_Texture();
	virtual ~CCubeTemple_Texture();

public:
	DeclRTTI(CCubeTemple_Texture, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CCubeTemple_Texture, CUBETEMPLE_TEX);

#endif // CubeTemple_Texture_h__
