#include "Triangle_Color.h"
using namespace Engine;

CTriangle_Color::CTriangle_Color()
{
}

CTriangle_Color::~CTriangle_Color()
{
}

void CTriangle_Color::SetColor(const D3DCOLOR& _color1, const D3DCOLOR& _color2, const D3DCOLOR& _color3)
{
	VTXCOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].dwColor	= _color1;
	pVertex[1].dwColor	= _color2;
	pVertex[2].dwColor	= _color3;

	m_pVB->Unlock();
}

HRESULT CTriangle_Color::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXCOL;
	m_dwVBSize	= sizeof(VTXCOL);
	m_iVBCnt	= 3;
	m_iPrimCnt	= 1;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;
	

	VTXCOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition = D3DXVECTOR3(0.f, 0.f, 0.5f);
		pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.f, -0.5f);
		pVertex[2].vPosition = D3DXVECTOR3(-0.5f, 0.f, -0.5f);

		for(int i = 0; i < 3; ++i)
			pVertex[i].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	}
	m_pVB->Unlock();

	return S_OK;
}