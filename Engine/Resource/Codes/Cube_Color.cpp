#include "Cube_Color.h"
using namespace Engine;

CCube_Color::CCube_Color()
{
}

CCube_Color::~CCube_Color()
{
}

HRESULT CCube_Color::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXCOL;
	m_dwVBSize	= sizeof(VTXCOL);
	m_iVBCnt	= 8;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 36;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	VTXCOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition = _vec3(-0.5f,	-0.5f,	-0.5f);
		pVertex[1].vPosition = _vec3(-0.5f, 0.5f,	-0.5f);
		pVertex[2].vPosition = _vec3(0.5f,	0.5f,	-0.5f);
		pVertex[3].vPosition = _vec3(0.5f,	-0.5f,	-0.5f);
		pVertex[4].vPosition = _vec3(-0.5f,	-0.5f,	0.5f);
		pVertex[5].vPosition = _vec3(-0.5f,	0.5f,	0.5f);
		pVertex[6].vPosition = _vec3(0.5f,	0.5f,	0.5f);
		pVertex[7].vPosition = _vec3(0.5f,	-0.5f,	0.5f);

		for(_uint i = 0; i < m_iVBCnt; ++i)
			pVertex[i].dwColor = D3DXCOLOR(float(rand()%100)/100.f, float(rand()%100)/100.f, float(rand()%100)/100.f, 1.f);
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		IndexBufferInitializer(pIndex, m_iIBCnt,
			0, 1, 2,		0, 2, 3,
			4, 6, 5,		4, 7, 6,
			4, 5, 1,		4, 1, 0,
			3, 2, 6,		3, 6, 7,
			1, 5, 6,		1, 6, 2,
			4, 0, 3,		4, 3, 7);
	}

	m_pIB->Unlock();

	return S_OK;
}