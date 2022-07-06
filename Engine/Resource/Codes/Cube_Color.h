#ifndef Cube_Color_h__
#define Cube_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube_Color : public CVIBuffer
{
protected:
	explicit CCube_Color();
	virtual ~CCube_Color();

public:
	DeclRTTI(CCube_Color, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CCube_Color, CUBE_COL);

#endif // Cube_Color_h__
