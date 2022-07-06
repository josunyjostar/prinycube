#include "stdafx.h"
#include "CubeTri_Texture.h"
using namespace Engine;

CCubeTri_Texture::CCubeTri_Texture()
{
	m_Matrial.Ambient	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

CCubeTri_Texture::~CCubeTri_Texture()
{
}

HRESULT CCubeTri_Texture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXNORMALTEX;
	m_dwVBSize	= sizeof(Engine::VTXNORMALTEX);
	m_iVBCnt	= 16;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 18;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	Engine::VTXNORMALTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);	//후면
		pVertex[1].vPosition	= _vec3( 0.5f,   0.5f,	-0.5f);
		pVertex[2].vPosition	= _vec3( 0.0f,	-0.5f,	-0.0f);

		pVertex[3].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//좌측면
		pVertex[4].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);
		pVertex[5].vPosition	= _vec3(-0.0f,	-0.5f,	-0.0f);

		pVertex[6].vPosition	= _vec3( 0.5f,	 0.5f,	-0.5f);	//우측면
		pVertex[7].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);
		pVertex[8].vPosition	= _vec3( 0.0f,	-0.5f,	 0.0f);

		pVertex[9].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);	//전면
		pVertex[10].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);
		pVertex[11].vPosition	= _vec3(-0.0f,	-0.5f,	 0.0f);

		pVertex[12].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//상단면
		pVertex[13].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);
		pVertex[14].vPosition	= _vec3( 0.5f,	 0.5f,	-0.5f);
		pVertex[15].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);

#define NORMAL(__START_INDEX, __NORMAL)			\
	pVertex[__START_INDEX+0].vNormal = __NORMAL;\
	pVertex[__START_INDEX+1].vNormal = __NORMAL;\
	pVertex[__START_INDEX+2].vNormal = __NORMAL;

		_vec3 vCross;

#define CROSS(__START_INDEX)			\
	D3DXVec3Cross(&vCross, &(pVertex[__START_INDEX+1].vPosition - pVertex[__START_INDEX+0].vPosition), &(pVertex[__START_INDEX+2].vPosition - pVertex[__START_INDEX+1].vPosition)); \
	D3DXVec3Normalize(&vCross, &vCross);

		CROSS(0)
		NORMAL(0,	vCross);
		CROSS(3)
		NORMAL(3,	vCross);
		CROSS(6)
		NORMAL(6,	vCross);
		CROSS(9)
		NORMAL(9,	vCross);
		NORMAL(12,	_vec3(0.f, 1.f, 0.f));
		pVertex[15].vNormal = _vec3(0.f, 1.f, 0.f);

#undef CROSS
#undef NORMAL
		
		for(int i = 0; i < 4; ++i) {
			int Index = i*3;
			pVertex[Index].vUV		= _vec2(0.5f, 1.f);
			pVertex[Index+1].vUV	= _vec2(1.f, 1.f);
			pVertex[Index+2].vUV	= _vec2(0.75f, 0.f);
		}
		pVertex[12].vUV = _vec2(0.0f, 0.0f);
		pVertex[13].vUV = _vec2(1.0f, 0.0f);
		pVertex[14].vUV = _vec2(1.0f, 1.0f);
		pVertex[15].vUV = _vec2(0.0f, 1.0f);
		
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		Engine::IndexBufferInitializer(pIndex, m_iIBCnt,
			0, 1, 2,		3, 4, 5,
			6, 7, 8,		9, 10, 11,
			12,13,14,		12,14,15);
	}

	m_pIB->Unlock();

	return S_OK;
}