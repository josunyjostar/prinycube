#ifndef NextCube_h__
#define NextCube_h__

#include "BasicCube.h"
#include "Player.h"

class CNextCube : public CBasicCube
{
protected:
	explicit CNextCube();
	virtual ~CNextCube();

public:
	DeclRTTI(CNextCube, CBasicCube);
	void Setting(RCP_STAGESCENE _StageScene, _vec3 _StartPoint, RCP_PLAYER _Player, const char* _NextSceneName, RCP_FADEINOUTFILTER _FadeInOut, RCP_SHADOWLIGHT _ShadowLight);

public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render();

protected:
	RCP_RC_TEX m_pMoon;
	RCP_TEXTURE	m_pMoonTexture;
	RCP_PLAYER m_pPlayer;
	RCP_SHADER m_pAlphaShader;
	RCP_FADEINOUTFILTER m_pFadeInOutFilter;

	const char* m_pNextSceneName;
	bool		m_bAction;

public:
	CREATE(CNextCube);
};

TYPEDEF_RCP(CNextCube, NEXTCUBE);

#endif // NextCube_h__