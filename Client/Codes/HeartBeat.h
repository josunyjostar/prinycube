#ifndef HeartBeat_h__
#define HeartBeat_h__

#include "Effect.h"

class CHeartBeat : public CEffect
{
protected:
	explicit CHeartBeat();
	virtual ~CHeartBeat();

public:
	DeclRTTI(CHeartBeat, CEffect);

public:
	virtual HRESULT Init();
	virtual void	Render();

private:
	RCP_D3DTEXTURE	m_pRenderTarget;
	RCP_SURFACE		m_pDepthStencil;

public:
	CREATE(CHeartBeat);
};

TYPEDEF_RCP(CHeartBeat, HEARTBEAT);

#endif // HeartBeat_h__
