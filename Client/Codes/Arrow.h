#ifndef Arrow_h__
#define Arrow_h__

#include "Effect.h"
#include "StageScene.h"

class CArrow : public CEffect
{
private:
	explicit CArrow();
	virtual ~CArrow();
	DeclRTTI(CArrow, CEffect);

public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render();
	virtual void CollisionEnter(Engine::COL_INFO& _ColInfo);

private:
	RCP_SHADER			m_pShadowCreateShader;
	RCP_D3DTEXTURE		m_pShadowRenderTarget[WALL_END];
	RCP_SURFACE			m_pShadowDepthStencil[WALL_END];
	RCP_SHADOWLIGHT		m_pShadowLight;
	float				m_fHideVal;
	float				m_fSwitchFoot;
	_matrix				m_matLightProj;

public:
	CREATE(CArrow);
	void Setting(VIEW_DIR _Dir, _vec3 _StartPoint);
	
};

TYPEDEF_RCP(CArrow, ARROW);
#endif // Arrow_h__
