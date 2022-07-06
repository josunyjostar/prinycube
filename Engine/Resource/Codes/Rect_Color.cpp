#include "Rect_Color.h"
using namespace Engine;

CRect_Color::CRect_Color()
{
}

CRect_Color::~CRect_Color()
{
}

HRESULT CRect_Color::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXCOL;
	m_dwVBSize	= sizeof(VTXCOL);
	m_iVBCnt	= 4;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 6;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;
	

	VTXCOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
		pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
		pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
		pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);

		for(int i = 0; i < 4; ++i)
			pVertex[i].dwColor	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else	IndexBufferInitializer(pIndex, m_iIBCnt, 0, 1, 2, 0, 2, 3);

	m_pIB->Unlock();

	return S_OK;
}