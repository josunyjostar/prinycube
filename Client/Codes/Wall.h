#ifndef Wall_h__
#define Wall_h__

#include "Include.h"
#include "ShadowLight.h"

class CWall : public Engine::CGameObject
{
protected:
	explicit CWall();
	virtual ~CWall();

public:
	DeclRTTI(CWall, Engine::CGameObject);

	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, int _TexIndex = 0);
	void SetDiffuse(float _fDiffuse)						{ m_fDiffuse = _fDiffuse; }
	void SetShadowLight(RCP_SHADOWLIGHT _pShadowLight)		{ m_pShadowLight = _pShadowLight; }
	void SetLightProj(const _matrix& _matLightProj)			{ m_matLightProj = _matLightProj; }
	void SetRenderTarget(RCP_D3DTEXTURE _RenderTarget)		{ m_pShadowRenderTarget = _RenderTarget; }
	void SetDepthBuffer(RCP_SURFACE _pDepthBuffer)			{ m_pShadowDepthStencil = _pDepthBuffer; }

public:
	virtual HRESULT		Init();
	virtual void		Render();

protected:
	RCP_DEVICE			m_pDev;
	RCP_TRANSFORM		m_pTransform;
	RCP_RC_TEX			m_pResource;
	RCP_TEXTURE			m_pTexture;
	RCP_RENDERER		m_pRenderer;
	RCP_SHADER			m_pColorShader;

	RCP_D3DTEXTURE		m_pShadowRenderTarget;
	RCP_SURFACE			m_pShadowDepthStencil;
	RCP_SHADER			m_pShadowApplyShader;
	RCP_SHADOWLIGHT		m_pShadowLight;
	_matrix				m_matLightProj;

	int					m_iTexIndex;
	float				m_fDiffuse;

public:
	CREATE(CWall);
};

TYPEDEF_RCP(CWall, WALL);

#endif // Wall_h__
