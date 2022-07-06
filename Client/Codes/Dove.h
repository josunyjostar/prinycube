#ifndef Dove_h__
#define Dove_h__

#include "AniEffect.h"

class CDove : public CAniEffect
{
public:
	enum FLYDIR   {DIR_LEFT,DIR_RIGHT};
	enum FLYTYPE  {TYPE_DIAGONAL,TYPE_WAVE}; // DIAGONAL ´ë°¢¼±
private:
	explicit CDove(void);
	virtual ~CDove(void);
public:
	DeclRTTI(CDove, CAniEffect);
public:
	void	SetDoveState(const _vec3& pStartPos,FLYDIR eDir,FLYTYPE eType,bool isAccel = true,float fCurveCycle = 0.2f,float fAmplitude=6.f, float fLifeTime = 10.f);
private:
	FLYTYPE		m_eFlyType;
	FLYDIR		m_eDir;

	float		m_fTheta;
	float		m_fCurveCycle;
	float		m_fAmplitude;
	float		m_fLifeTime;

private:
	virtual HRESULT Init();
	virtual void	Update();

public:
	CREATE(CDove);
};

TYPEDEF_RCP(CDove,DOVE);

#endif // Dove_h__
