#include "Plane_Color.h"
using namespace Engine;

CPlane_Color::CPlane_Color()
:m_iX(1)
,m_iY(1)
{
}

CPlane_Color::~CPlane_Color()
{
}

void CPlane_Color::SetPlaneSize(_uint _iX, _uint _iY)
{
	if(_iX == 0 || _iY == 0) return;

	m_iX = _iX;
	m_iY = _iY;

	this->Release_VIBuffer();
	this->CreateBuffer();
}

HRESULT CPlane_Color::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	int maxSize = m_iX * m_iY;

	m_dwFVF		= D3DFVF_VTXCOL;
	m_dwVBSize	= sizeof(VTXCOL);
	m_iVBCnt	= maxSize*4;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= maxSize*6;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;



	VTXCOL* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		float LTX = -(m_iX * 0.5f);
		float LTY = m_iY * 0.5f;

		for(int i = 0; i < maxSize; ++i)
		{
			int Index = i*4;
			float x = float(i%m_iX);
			float y = float(i/m_iX);

			pVertex[Index].vPosition	= _vec3(LTX + x,		0.f, LTY - y);
			pVertex[Index+1].vPosition	= _vec3(LTX + x + 1.f,	0.f, LTY - y);
			pVertex[Index+2].vPosition	= _vec3(LTX + x + 1.f,	0.f, LTY - y - 1.f);
			pVertex[Index+3].vPosition	= _vec3(LTX + x,		0.f, LTY - y - 1.f);

			pVertex[Index].dwColor		= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			pVertex[Index+1].dwColor	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			pVertex[Index+2].dwColor	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			pVertex[Index+3].dwColor	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}			
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		for(int i = 0; i < maxSize; ++i)
		{
			int Index		= i*6;
			pIndex[Index]	= i*4;
			pIndex[Index+1] = pIndex[Index]+1;
			pIndex[Index+2] = pIndex[Index]+2;

			pIndex[Index+3] = pIndex[Index];
			pIndex[Index+4] = pIndex[Index]+2;
			pIndex[Index+5] = pIndex[Index]+3;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}