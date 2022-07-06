#ifndef TempleLight_h__
#define TempleLight_h__

#include "BasicCube.h"
#include "Player.h"

class CTempleLight : public CBasicCube
{
protected:
	explicit CTempleLight();
	virtual ~CTempleLight();

public:
	DeclRTTI(CTempleLight, CBasicCube);

	void AddBright(float _fBright) { m_fBright += _fBright; }
	void SetHide(bool _bHide, bool _bInit = false);
	void Setting(RCP_STAGESCENE _StageScene, _vec3 _StartPoint, RCP_PLAYER _Player, const char* _NextSceneName, RCP_FADEINOUTFILTER _FadeInOut, RCP_SHADOWLIGHT _ShadowLight);
	void SetFake(bool _fake) {m_bFake = _fake;}

public:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();

private:
	float	m_fBright;
	RCP_PLAYER m_pPlayer;
	RCP_FADEINOUTFILTER m_pFadeInOutFilter;
	const char* m_pNextSceneName;
	bool		m_bAction;
	bool		m_bFake;
public:
	CREATE(CTempleLight);
};

TYPEDEF_RCP(CTempleLight, TEMPLELIGHT);

#endif // TempleLight_h__
