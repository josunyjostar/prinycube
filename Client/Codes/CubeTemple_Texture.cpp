#include "stdafx.h"
#include "CubeTemple_Texture.h"
using namespace Engine;

CCubeTemple_Texture::CCubeTemple_Texture()
{
	m_Matrial.Ambient	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

CCubeTemple_Texture::~CCubeTemple_Texture()
{
}

HRESULT CCubeTemple_Texture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXNORMALTEX;
	m_dwVBSize	= sizeof(Engine::VTXNORMALTEX);
	m_iVBCnt	= 40;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 42;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	Engine::VTXNORMALTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition	= _vec3(-0.5f,	 0.5f,	 0.0f);	//왼쪽뒷다리 좌
		pVertex[1].vPosition	= _vec3(-0.5f,   0.5f,	-0.5f);
		pVertex[2].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);

		pVertex[3].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);	//후
		pVertex[4].vPosition	= _vec3( 0.0f,   0.5f,	-0.5f);
		pVertex[5].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);

		pVertex[6].vPosition	= _vec3( 0.0f,   0.5f,	-0.5f);	//내
		pVertex[7].vPosition	= _vec3(-0.5f,	 0.5f,	 0.0f);
		pVertex[8].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);


		pVertex[9].vPosition	= _vec3( 0.5f,	 0.5f,	-0.5f);	//오른쪽뒷다리 우
		pVertex[10].vPosition	= _vec3( 0.5f,   0.5f,	 0.0f);
		pVertex[11].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);

		pVertex[12].vPosition	= _vec3( 0.0f,	 0.5f,	-0.5f);	//후
		pVertex[13].vPosition	= _vec3( 0.5f,   0.5f,	-0.5f);
		pVertex[14].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);

		pVertex[15].vPosition	= _vec3( 0.5f,   0.5f,	 0.0f);	//내
		pVertex[16].vPosition	= _vec3( 0.0f,	 0.5f,	-0.5f);
		pVertex[17].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);

		pVertex[18].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//왼쪽앞다리 좌
		pVertex[19].vPosition	= _vec3(-0.5f,   0.5f,	 0.0f);
		pVertex[20].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);
									
		pVertex[21].vPosition	= _vec3( 0.0f,   0.5f,	 0.5f);	//전
		pVertex[22].vPosition	= _vec3(-0.5f,   0.5f,	 0.5f);
		pVertex[23].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);
									
		pVertex[24].vPosition	= _vec3(-0.5f,   0.5f,	 0.0f);	//내
		pVertex[25].vPosition	= _vec3( 0.0f,   0.5f,	 0.5f);
		pVertex[26].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);

		pVertex[27].vPosition	= _vec3( 0.5f,	 0.5f,	 0.0f);	//오른쪽앞다리 우
		pVertex[28].vPosition	= _vec3( 0.5f,   0.5f,	 0.5f);
		pVertex[29].vPosition	= _vec3( 0.5f,	-0.5f,	 0.5f);
									
		pVertex[30].vPosition	= _vec3( 0.5f,   0.5f,	 0.5f);	//전
		pVertex[31].vPosition	= _vec3( 0.0f,   0.5f,	 0.5f);
		pVertex[32].vPosition	= _vec3( 0.5f,	-0.5f,	 0.5f);
									
		pVertex[33].vPosition	= _vec3( 0.0f,   0.5f,	 0.5f);	//내
		pVertex[34].vPosition	= _vec3( 0.5f,	 0.5f,	 0.0f);
		pVertex[35].vPosition	= _vec3( 0.5f,	-0.5f,	 0.5f);

		pVertex[36].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//상단면
		pVertex[37].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);
		pVertex[38].vPosition	= _vec3( 0.5f,	 0.5f,	-0.5f);
		pVertex[39].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);

#define NORMAL(__START_INDEX, __NORMAL)			\
		pVertex[__START_INDEX+0].vNormal = __NORMAL;\
		pVertex[__START_INDEX+1].vNormal = __NORMAL;\
		pVertex[__START_INDEX+2].vNormal = __NORMAL;

		_vec3 vCross;
#define CROSS(__START_INDEX)			\
		D3DXVec3Cross(&vCross, &(pVertex[__START_INDEX+1].vPosition - pVertex[__START_INDEX+0].vPosition), &(pVertex[__START_INDEX+2].vPosition - pVertex[__START_INDEX+1].vPosition)); \
		D3DXVec3Normalize(&vCross, &vCross);

		for(int i = 0; i < 33; i+=3)
		{
			CROSS(i)
			NORMAL(i, vCross)
		}

		NORMAL(36,	_vec3(0.f, 1.f, 0.f))
		pVertex[39].vNormal = _vec3(0.f, 1.f, 0.f);
		
		for(int i = 0; i < 12; ++i) {
			int Index = i*3;
			pVertex[Index].vUV		= _vec2(0.5f, 1.f);
			pVertex[Index+1].vUV	= _vec2(1.f, 1.f);
			pVertex[Index+2].vUV	= _vec2(0.75f, 0.f);
		}

		pVertex[36].vUV = _vec2(0.0f, 0.0f);
		pVertex[37].vUV = _vec2(1.0f, 0.0f);
		pVertex[38].vUV = _vec2(1.0f, 1.0f);
		pVertex[39].vUV = _vec2(0.0f, 1.0f);

#undef CROSS
#undef NORMAL
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		Engine::IndexBufferInitializer(pIndex, m_iIBCnt,
			0, 1, 2,		3, 4, 5,
			6, 7, 8,		9, 10,11,
			12,13,14,		15,16,17,
			18,19,20,		21,22,23,
			24,25,26,		27,28,29,
			30,31,32,		33,34,35,
			36,37,38,		36,38,39 );
	}		
	m_pIB->Unlock();

	return S_OK;
}