#ifndef Triangle_Color_h__
#define Triangle_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriangle_Color : public CVIBuffer
{
protected:
	explicit CTriangle_Color();
	virtual ~CTriangle_Color();

public:
	void SetColor(const D3DCOLOR& _color1, const D3DCOLOR& _color2, const D3DCOLOR& _color3);

public:
	DeclRTTI(CTriangle_Color, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CTriangle_Color, TRI_COL);

#endif // Triangle_Color_h__
