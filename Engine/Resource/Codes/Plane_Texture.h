#ifndef Plane_Color_h__
#define Plane_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPlane_Texture : public CVIBuffer
{
protected:
	explicit CPlane_Texture();
	virtual ~CPlane_Texture();

public:
	DeclRTTI(CPlane_Texture, CVIBuffer);
	friend CComponent;

	void SetPlaneSize(_uint _iX, _uint _iY);

public:
	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);

protected:
	_uint	m_iX;
	_uint	m_iY;
};

END

TYPEDEF_RCP(Engine::CPlane_Texture, PLANE_TEX);

#endif // Plane_Color_h__
