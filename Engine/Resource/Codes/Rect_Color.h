#ifndef Rect_Color_h__
#define Rect_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRect_Color : public CVIBuffer
{
protected:
	explicit CRect_Color();
	virtual ~CRect_Color();

public:
	DeclRTTI(CRect_Color, CVIBuffer);
	friend CComponent;

	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CRect_Color, RC_COL);

#endif // Rect_Color_h__
