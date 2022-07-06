#ifndef Dust_h__
#define Dust_h__

#include "Effect.h"
#include "Player.h"

class CDust : public CEffect
{
private:
	explicit CDust(void);
	virtual ~CDust(void);
public:
	DeclRTTI(CDust, CEffect);
public:
	virtual HRESULT Init();
private:
	RCP_TEXTURE			m_pTexture;
	RCP_RENDERER		m_pRenderer;
public:
	CREATE(CDust);
};

TYPEDEF_RCP(CDust,DUST);

#endif // Dust_h__
