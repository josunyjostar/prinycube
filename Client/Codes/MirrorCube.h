#ifndef MirrorCube_h__
#define MirrorCube_h__

#include "PickUpCube.h"
#include "Player.h"

class CMirrorCube : public CPickUpCube
{
protected:
	explicit CMirrorCube();
	virtual ~CMirrorCube();

public:
	DeclRTTI(CMirrorCube, CBasicCube);

	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();
	virtual void	Pick(RCP_PLAYER _pPlayer);

	void SetCanPick(_bool b) { m_bCanPick = b; if(b) ((RCP_CUBE)m_pCollider)->SetSize(_vec3(1,1,1)); }
	void SetColor(_vec3 vColor) { m_vColor.x = vColor.x+0.2f; m_vColor.y = vColor.y+0.2f; m_vColor.z = vColor.z+0.2f; }

	bool GetCanPick() { return m_bCanPick; }

private:

	RCP_SHADER m_pMetalShader;

	_matrix m_matOrigin;
	_float	m_fYScale;
	_bool	m_bState;
	_bool	m_bCanPick;


public:
	CREATE(CMirrorCube);
};

TYPEDEF_RCP(CMirrorCube, MIRRORCUBE);

#endif // MirrorCube_h__
