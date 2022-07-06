#ifndef BridgeCube_h__
#define BridgeCube_h__

#include "BasicCube.h"
#include "Player.h"
#include "Cylinder_Texture.h"

class CBridgeCube : public CBasicCube
{
protected:
	explicit CBridgeCube();
	virtual ~CBridgeCube();

public:
	DeclRTTI(CBridgeCube, CBasicCube);

	void			SetPlayer(RCP_PLAYER _pPlayer) { m_pPlayer = _pPlayer; }
	void			SetViewDir(VIEW_DIR _eDir);

public:
	virtual HRESULT Init();
	virtual void	Update();
	void			RightRotate();
	void			LeftRotate();

private:
	RCP_PLAYER		m_pPlayer;
	VIEW_DIR		m_eDir;
	VIEW_DIR		m_eStartDir;

public:
	CREATE(CBridgeCube);		
};

TYPEDEF_RCP(CBridgeCube, BRIDGECUBE);

#endif // BridgeCube_h__
