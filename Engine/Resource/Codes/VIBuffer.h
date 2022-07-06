#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CResource
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	CVIBuffer& operator=(const CVIBuffer& rhs);

public:
	DeclRTTI(CVIBuffer, CResource);

	void			SetMatrial(const D3DMATERIAL9& _Matrial)	{ m_Matrial = _Matrial; }

	RCP_VB			GetVB() const								{ return m_pVB; }
	RCP_IB			GetIB() const								{ return m_pIB; }
	D3DMATERIAL9	GetMatrial() const							{ return m_Matrial; }
	_ulong			GetFVF() const								{ return m_dwFVF; }
	_ulong			GetVBSize() const							{ return m_dwVBSize; }
	_ulong			GetVBCnt() const							{ return m_iVBCnt;}
	_ulong			GetIBCnt() const							{ return m_iIBCnt;}

	void			VBCopy(void** _ppVertex, const CVIBuffer* _pVIBuffer);
	void			IBCopy(void** _ppIndex, const CVIBuffer* _pVIBuffer);

public:
	virtual void	Render();
	virtual HRESULT InitResource(RCP_DEVICE& _pGraphicDev);

protected:
	LPDIRECT3DVERTEXBUFFER9 	m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	_ulong						m_dwFVF;
	_ulong						m_dwVBSize;
	_ulong						m_dwIBSize;
	_uint						m_iVBCnt;
	_uint						m_iIBCnt;
	_uint						m_iPrimCnt;
	D3DMATERIAL9				m_Matrial;

protected:
	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL) { return S_OK; }
	HRESULT			Create_VIBuffer();
	void			Release_VIBuffer();
};

END

TYPEDEF_RCP(Engine::CVIBuffer, VIBUFFER);

#endif // VIBuffer_h__
