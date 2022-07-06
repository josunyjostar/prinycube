#ifndef Fish_h__
#define Fish_h__

#include "Effect.h"

class CFish : public CEffect
{
public:
	enum SWIMDIR	{DIR_LEFT,DIR_RIGHT};
	enum SWIMTYPE   {TYPE_ZIGZAG}; // 
private:
	explicit CFish(void);
	virtual ~CFish(void);
public:
	DeclRTTI(CFish, CEffect);
public:
	void	SetState(const _vec3& pStartPos,SWIMDIR eDir,SWIMTYPE eType,float fLifeTime = 60.f);
private:
	SWIMTYPE	m_eSwimType;
	SWIMDIR		m_eDir;

	float		m_fTheta;
	float		m_fLifeTime;


private:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();

public:
	CREATE(CFish);
};

TYPEDEF_RCP(CFish,FISH);

#endif // Fish_h__
