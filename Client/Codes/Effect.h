#ifndef Effect_h__
#define Effect_h__

#include "Include.h"

class CEffect : public Engine::CGameObject
{
protected:
	explicit CEffect();
	virtual ~CEffect();

public:
	DeclRTTI(CEffect, Engine::CGameObject);

	void			SetMoveDir(_vec3 _vMoveDir)			{ m_vMoveDir = _vMoveDir; }
	void			SetMoveAccel(_float _fMoveAccel)	{ m_fMoveAccel = _fMoveAccel; }
	void			SetScalling(_vec3 _vScalling)		{ m_vScalling = _vScalling; }
	void			SetScaleAccel(_float _fScaleAccel)	{ m_fScaleAccel = _fScaleAccel; }
	void			SetAxis(_vec3 _vAxis)				{ m_vAxis = _vAxis; }
	void			SetRotateSpeed(_float _fRotateSpeed){ m_fRotateSpeed = _fRotateSpeed; }
	void			SetRotateAccel(_float _fRotateAccel){ m_fRotateAccel = _fRotateAccel; }
	void			SetRotate(_vec3 _vAxis, _float _fRotateSpeed);
	void			SetLifeTime(float _fLifeTime)		{ m_fMaxLifeTime = m_fLifeTime = _fLifeTime; m_bUseLifeTime = true; }
	void			SetBillboard(bool _bUseBillboard)	{ m_bBillboard = _bUseBillboard; }
	void			SetTexture(RCP_TEXTURE _pTexture)	{ m_pTexture = _pTexture; }
	void			SetTexture(int _iContainerIdx, const _char* _pResourceTag);
	void			SetColor(_vec4 _vColor)				{ m_vColor = _vColor; }
	void			SetTargetColor(_vec4 _vTargetColor) { m_vTargetColor = _vTargetColor; }
	virtual void	SetResource(RCP_RESOURCE _pResource){ m_pResource = _pResource; }

	RCP_RESOURCE	GetResource() const					{ return m_pResource; }
	RCP_TEXTURE		GetTexture() const					{ return m_pTexture; }
	bool			GetBillboard() const				{ return m_bBillboard; }

public:
	virtual void	Update();
	virtual void	Render();

protected:
	RCP_TIMER		m_pTimer;
	RCP_DEVICE		m_pGraphicDev;
	RCP_TRANSFORM	m_pTransform;
	RCP_RENDERER	m_pRenderer;
	RCP_RESOURCE	m_pResource;
	RCP_TEXTURE		m_pTexture;
	int				m_iTextureIndex;
	RCP_SHADER		m_pShader;
	bool			m_bBillboard;
	float			m_fDeltaTime;	

	bool			m_bUseLifeTime;
	float			m_fMaxLifeTime;
	float			m_fLifeTime;	

	_vec4			m_vColor;
	_vec4			m_vTargetColor;
	_vec3			m_vMoveDir;
	_float			m_fMoveAccel;
	_vec3			m_vScalling;
	_float			m_fScaleAccel;
	_vec3			m_vAxis;
	_float			m_fRotateSpeed;
	_float			m_fRotateAccel;

public:
	CREATE(CEffect);
};

TYPEDEF_RCP(CEffect, EFFECT);

#endif // Effect_h__
