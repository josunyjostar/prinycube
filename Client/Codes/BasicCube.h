#ifndef BasicCube_h__
#define BasicCube_h__

#include "Include.h"
#include "StageScene.h"

class CBasicCube : public Engine::CGameObject
{
protected:
	explicit CBasicCube();
	virtual ~CBasicCube();

public:
	DeclRTTI(CBasicCube, Engine::CGameObject);

	void SetShadow(RCP_STAGESCENE _pStageScene, const _vec3& _vCreatePos, RCP_SHADOWLIGHT _pShadowLight);
	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag);
	void SetParent(RCP_TRANSFORM _parent)					{ m_pParent = _parent; }
	void SetLocalWorld(const _matrix& _matLocalWorld)		{ m_matLocalWorld = _matLocalWorld; }
	void SetLocalWorld()									{ m_matLocalWorld = m_pTransform->GetWorldMatrix(); }
	void SetLocalScale(const _vec3& _vScale);
	void SetLocalPos(const _vec3& _vLocalPos)				{ memcpy(m_matLocalWorld.m[3], &_vLocalPos, sizeof(_vec3)); }
	void SetLocalRotate(const _vec3& _vAxis, const float& _fAngle);
	bool GetHide() const									{ return m_bHide; }
	bool GetStartHide()										{ return m_bStartHide; }
	bool GetNormalAndDest(const _ray& _vWorldRay, _vec3& vNormal, _float& fMinDest);
	
	virtual void SetHide(bool _bHide, bool _bInit = false);

public:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();

public:
	RCP_TIMER		m_pTimer;
	RCP_DEVICE		m_pGraphicDev;
	RCP_TEXTURE		m_pTexture;
	RCP_COLLIDER	m_pCollider;
	RCP_RENDERER	m_pRenderer;
	RCP_RESOURCE	m_pResource;
	RCP_TRANSFORM	m_pTransform;
	RCP_TRANSFORM	m_pParent;
	_matrix			m_matLocalWorld;
	RCP_SHADER		m_pColorShader;
	_vec4			m_vColor;

	RCP_SHADER		m_pShadowCreateShader;
	RCP_D3DTEXTURE	m_pShadowRenderTarget[WALL_END];
	RCP_SURFACE		m_pShadowDepthStencil[WALL_END];
	RCP_SHADOWLIGHT	m_pShadowLight;
	_matrix			m_matLightProj;

	bool			m_bHide;
	float			m_fHideVal;
	bool			m_bStartHide;

public:
	CREATE(CBasicCube);
};

TYPEDEF_RCP(CBasicCube, BASICCUBE);

#endif // BasicCube_h__
