#include "Cube_CubeTexture.h"
using namespace Engine;

CCube_CubeTexture::CCube_CubeTexture()
{
}

CCube_CubeTexture::~CCube_CubeTexture()
{
}

HRESULT CCube_CubeTexture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXCUBETEX;
	m_dwVBSize	= sizeof(VTXCUBETEX);
	m_iVBCnt	= 8;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 36;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	VTXCUBETEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition = _vec3(-0.5f,	-0.5f,	-0.5f);
		pVertex[1].vPosition = _vec3(-0.5f,  0.5f,	-0.5f);
		pVertex[2].vPosition = _vec3( 0.5f,	 0.5f,	-0.5f);
		pVertex[3].vPosition = _vec3( 0.5f,	-0.5f,	-0.5f);
		pVertex[4].vPosition = _vec3(-0.5f,	-0.5f,	 0.5f);
		pVertex[5].vPosition = _vec3(-0.5f,	 0.5f,	 0.5f);
		pVertex[6].vPosition = _vec3( 0.5f,	 0.5f,	 0.5f);
		pVertex[7].vPosition = _vec3( 0.5f,	-0.5f,	 0.5f);

		for(int i = 0; i < 8; ++i)
			pVertex[i].vUV = pVertex[i].vPosition;
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