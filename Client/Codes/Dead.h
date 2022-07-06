#ifndef Dead_h__
#define Dead_h__

#include "Include.h"

class CDead : public Engine::CGameObject
{
protected:
	explicit CDead();
	virtual ~CDead();

public:
	DeclRTTI(CDead, Engine::CGameObject);

	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, VIEW_DIR _eDir);
	void SetIndex(const _int& _iIndex)	{ m_iIndex = _iIndex; }
	_vec3 GetCreatePos() const;

public:
	virtual HRESULT		Init();
	virtual void		Render();

protected:
	RCP_DEVICE			m_pDev;
	RCP_TRANSFORM		m_pTransform;
	RCP_RC_TEX			m_pResource;
	RCP_TEXTURE			m_pTexture;
	RCP_RENDERER		m_pRenderer;
	RCP_SHADER			m_pShader;

	VIEW_DIR			m_eDir;
	int					m_iIndex;

	_float				m_fHideVal;

public:
	CREATE(CDead);
};

TYPEDEF_RCP(CDead, DEAD);

#endif // Dead_h__
