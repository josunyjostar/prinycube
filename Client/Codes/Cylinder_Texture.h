#ifndef Cylinder_Texture_h__
#define Cylinder_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class CCylinder_Texture : public Engine::CVIBuffer
{
protected:
	explicit CCylinder_Texture();
	virtual ~CCylinder_Texture();

public:
	DeclRTTI(CCylinder_Texture, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CCylinder_Texture, CYLINDER_TEX);

#endif // Cylinder_Texture_h__
