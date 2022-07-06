#ifndef Rect_Texture_h__
#define Rect_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRect_Texture : public CVIBuffer
{
protected:
	explicit CRect_Texture();
	virtual ~CRect_Texture();

public:
	DeclRTTI(CRect_Texture, CVIBuffer);
	friend CComponent;

	void SetOffset(float _fU, float _fV);
	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);
};

END

TYPEDEF_RCP(Engine::CRect_Texture, RC_TEX);

#endif // Rect_Texture_h__
