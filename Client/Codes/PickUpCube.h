#ifndef PickCube_h__
#define PickCube_h__

#include "BasicCube.h"
#include "Player.h"

class CPickUpCube : public CBasicCube
{
protected:
	explicit CPickUpCube();
	virtual ~CPickUpCube();

public:
	DeclRTTI(CPickUpCube, CBasicCube);

	virtual void SetHide(bool _bHide, bool _bInit = false);

public:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Pick(RCP_PLAYER _pPlayer);
	void			Throw(const _vec3& _vTargetPos);

protected:
	RCP_PLAYER	m_pPlayer;
	_matrix		m_matTarget;
	VIEW_DIR	m_ePrevDir;
	VIEW_DIR	m_eDir;

	bool		m_bThrow;
	float		m_fThrowTime;
	float		m_fEndTime;
	float		m_fVelocity;
	float		m_fGravity;
	float		m_fXZDist;
	float		m_fDisplacement;
	_vec3		m_vMove;
	_vec3		m_vStartPos;
	_vec3		m_vTargetPos;

public:
	CREATE(CPickUpCube);
};

TYPEDEF_RCP(CPickUpCube, PICKUPCUBE);

#endif // PickCube_h__
