#ifndef ObjectTexRect_h__
#define ObjectTexRect_h__

#include "Include.h"

class CObjectTexRect : public Engine::CGameObject
{
protected:
	explicit CObjectTexRect();
	virtual ~CObjectTexRect();

public:
	DeclRTTI(CObjectTexRect, Engine::CGameObject);

	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag);
	void SetBillboard(bool _bUse)		{ m_bBillboard = _bUse; }

	RCP_RC_TEX GetResource() const		{ return m_pResource; }
	RCP_TRANSFORM GetTransform() const	{ return m_pTransform; }

public:
	virtual HRESULT Init();
	virtual void	Render();

public:
	RCP_DEVICE		m_pGraphicDev;
	RCP_RC_TEX		m_pResource;
	RCP_TEXTURE		m_pTexture;
	RCP_TRANSFORM	m_pTransform;
	bool			m_bBillboard;

public:
	CREATE(CObjectTexRect);
};

TYPEDEF_RCP(CObjectTexRect, OBJECTTEXRECT);

#endif // ObjectTexRect_h__
