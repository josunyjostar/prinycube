#ifndef FootPrint_h__
#define FootPrint_h__

#include "Effect.h"
#include "Player.h"
class CFootPrint : public CEffect
{
private:
	explicit CFootPrint(void);
	virtual ~CFootPrint(void);
public:
	DeclRTTI(CFootPrint, CEffect);
public:
	virtual HRESULT Init();
private:
	RCP_PLAYER					m_pPlayer;
public:
	CREATE(CFootPrint);
};

TYPEDEF_RCP(CFootPrint,FOOTRPINT);

#endif // FootPrint_h__
