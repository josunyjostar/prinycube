#include "VIBuffer.h"
using namespace Engine;

CVIBuffer::CVIBuffer()
:m_pIB(NULL)
,m_pVB(NULL)
,m_dwFVF(0)
,m_dwVBSize(0)
,m_dwIBSize(0)
,m_iVBCnt(0)
,m_iIBCnt(0)
,m_iPrimCnt(0)
{
	ZeroMemory(&m_Matrial, sizeof(D3DMATERIAL9));
	m_Matrial.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Matrial.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Matrial.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_Matrial.Power		= 1.f;
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
:CResource(rhs)
,m_pIB(NULL)
,m_pVB(NULL)
,m_Matrial(rhs.m_Matrial)
{
	if(m_pGraphicDev) this->CreateBuffer(&rhs);
}

CVIBuffer::~CVIBuffer()
{
	this->Release_VIBuffer();
}


CVIBuffer& CVIBuffer::operator=(const CVIBuffer& rhs)
{
	CResource::operator=(rhs);
	if(m_pGraphicDev)
	{
		this->Release_VIBuffer();
		m_Matrial = rhs.m_Matrial;
		if(FAILED(this->CreateBuffer(&rhs)))
			return *this;
	}
	return *this;
}

void CVIBuffer::VBCopy(void** _ppVertex, const CVIBuffer* _pVIBuffer)
{
	if(_pVIBuffer->m_pVB) {
		void* pVertex = NULL;
		_pVIBuffer->m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		memcpy(*_ppVertex, pVertex, m_dwVBSize*m_iVBCnt);

		_pVIBuffer->m_pVB->Unlock();
	}
}

void CVIBuffer::IBCopy(void** _ppIndex, const CVIBuffer* _pVIBuffer)
{
	if(_pVIBuffer->m_pIB) {
		void* pVertex = NULL;
		_pVIBuffer->m_pIB->Lock(0, 0, (void**)&pVertex, 0);

		memcpy(*_ppIndex, pVertex, sizeof(WORD)*m_iIBCnt);

		_pVIBuffer->m_pIB->Unlock();
	}
}

void CVIBuffer::Render()
{
	m_pGraphicDev->SetMaterial(&m_Matrial);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVBSize);
	m_pGraphicDev->SetFVF(m_dwFVF);

	if(m_pIB) {
		m_pGraphicDev->SetIndices(m_pIB);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVBCnt, 0, m_iPrimCnt);
	}
	else
		m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_iPrimCnt);
}

HRESULT CVIBuffer::InitResource(RCP_DEVICE& _pGraphicDev)
{
	m_pGraphicDev = _pGraphicDev;
	if(FAILED(this->CreateBuffer())) return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_VIBuffer()
{
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVBSize*m_iVBCnt, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		ERR_MSG("VB Created Failed");
		return E_FAIL;
	}
	if(m_iIBCnt)
	{
		if(FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIBSize*m_iIBCnt, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		{	
			ERR_MSG("IB Created Failed");
			return E_FAIL;
		}
		m_iPrimCnt = m_iIBCnt/3;
	}

	return S_OK;
}

void CVIBuffer::Release_VIBuffer()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}