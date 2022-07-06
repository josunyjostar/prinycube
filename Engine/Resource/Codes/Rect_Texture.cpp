#include "Rect_Texture.h"
using namespace Engine;

CRect_Texture::CRect_Texture()
{
}

CRect_Texture::~CRect_Texture()
{
}

void CRect_Texture::SetOffset(float _fU, float _fV)
{
	VTXTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-_fU, 1.f-_fV, 0.f);
	pVertex[1].vPosition = _vec3(1.f-_fU, 1.f-_fV, 0.f);
	pVertex[2].vPosition = _vec3(1.f-_fU, -_fV, 0.f);
	pVertex[3].vPosition = _vec3(-_fU, -_fV, 0.f);

	m_pVB->Unlock();
}

HRESULT CRect_Texture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXTEX;
	m_dwVBSize	= sizeof(VTXTEX);
	m_iVBCnt	= 4;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 6;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	VTXTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
		pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
		pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
		pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);

		pVertex[0].vUV			= _vec2(0.f, 0.f);
		pVertex[1].vUV			= _vec2(1.f, 0.f);
		pVertex[2].vUV			= _vec2(1.f, 1.f);
		pVertex[3].vUV			= _vec2(0.f, 1.f);
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else	IndexBufferInitializer(pIndex, m_iIBCnt, 0, 1, 2, 0, 2, 3);

	m_pIB->Unlock();

	return S_OK;
}