#include "stdafx.h"
#include "PrismTri_Texture.h"
using namespace Engine;

CPrismTri_Texture::CPrismTri_Texture()
{
	m_Matrial.Ambient	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

CPrismTri_Texture::~CPrismTri_Texture()
{
}

HRESULT CPrismTri_Texture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXNORMALTEX;
	m_dwVBSize	= sizeof(Engine::VTXNORMALTEX);
	m_iVBCnt	= 18;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 24;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	Engine::VTXNORMALTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);	//후면
		pVertex[1].vPosition	= _vec3( 0.5f,   0.5f,	-0.5f);
		pVertex[2].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);
		pVertex[3].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);

		pVertex[4].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//좌측면
		pVertex[5].vPosition	= _vec3(-0.5f,   0.5f,	-0.5f);
		pVertex[6].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);
		pVertex[7].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);

		pVertex[8].vPosition	= _vec3( 0.5f,   0.5f,	-0.5f);	//경사면
		pVertex[9].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);
		pVertex[10].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);
		pVertex[11].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);

		pVertex[12].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//상단면
		pVertex[13].vPosition	= _vec3( 0.5f,   0.5f,	-0.5f);
		pVertex[14].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);

		pVertex[15].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);	//하단면
		pVertex[16].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);
		pVertex[17].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);

#define NORMAL4(__START_INDEX, __NORMAL)			\
	pVertex[__START_INDEX+0].vNormal = __NORMAL;\
	pVertex[__START_INDEX+1].vNormal = __NORMAL;\
	pVertex[__START_INDEX+2].vNormal = __NORMAL;\
	pVertex[__START_INDEX+3].vNormal = __NORMAL;

#define NORMAL3(__START_INDEX, __NORMAL)			\
	pVertex[__START_INDEX+0].vNormal = __NORMAL;\
	pVertex[__START_INDEX+1].vNormal = __NORMAL;\
	pVertex[__START_INDEX+2].vNormal = __NORMAL;
		
		_vec3 vCross;

#define CROSS(__START_INDEX)			\
	D3DXVec3Cross(&vCross, &(pVertex[__START_INDEX+1].vPosition - pVertex[__START_INDEX+0].vPosition), &(pVertex[__START_INDEX+2].vPosition - pVertex[__START_INDEX+1].vPosition)); \
	D3DXVec3Normalize(&vCross, &vCross);

		CROSS(0)
		NORMAL4(0,	vCross);
		CROSS(4)
		NORMAL4(4,	vCross);
		CROSS(8)
		NORMAL4(8,	vCross);
		NORMAL3(12,	_vec3(0.f, 1.f, 0.f));
		NORMAL3(15,	_vec3(0.f, -1.f, 0.f));

#undef CROSS
#undef NORMAL3
#undef NORMAL4
		
		for(int i = 0; i < 3; ++i) {
			int Index = i*4;
			pVertex[Index].vUV		= _vec2(0.f, 0.f);
			pVertex[Index+1].vUV	= _vec2(1.f, 0.f);
			pVertex[Index+2].vUV	= _vec2(1.f, 1.f);
			pVertex[Index+3].vUV	= _vec2(0.f, 1.f);
		}
		pVertex[12].vUV = _vec2(1.0f, 1.0f);
		pVertex[13].vUV = _vec2(1.0f, 0.0f);
		pVertex[14].vUV = _vec2(0.0f, 0.0f);

		pVertex[15].vUV = _vec2(0.0f, 1.0f);
		pVertex[16].vUV = _vec2(0.0f, 0.0f);
		pVertex[17].vUV = _vec2(1.0f, 0.0f);
		
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		Engine::IndexBufferInitializer(pIndex, m_iIBCnt,
			0, 1, 2,		0, 2, 3,
			4, 5, 6,		4, 6, 7,
			8, 9,10,		8,10,11,
			12,13,14,		15,16,17);
	}

	m_pIB->Unlock();

	return S_OK;
}