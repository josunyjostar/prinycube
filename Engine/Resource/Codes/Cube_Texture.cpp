#include "Cube_Texture.h"
using namespace Engine;

CCube_Texture::CCube_Texture()
{
	m_Matrial.Ambient	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

CCube_Texture::~CCube_Texture()
{
}

HRESULT CCube_Texture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	m_dwFVF		= D3DFVF_VTXNORMALTEX;
	m_dwVBSize	= sizeof(VTXNORMALTEX);
	m_iVBCnt	= 24;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= 36;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	VTXNORMALTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		pVertex[0].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);	//후면
		pVertex[1].vPosition	= _vec3( 0.5f,   0.5f,	-0.5f);
		pVertex[2].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);
		pVertex[3].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);

		pVertex[4].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//좌측면
		pVertex[5].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);
		pVertex[6].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);
		pVertex[7].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);

		pVertex[8].vPosition	= _vec3( 0.5f,	 0.5f,	-0.5f);	//우측면
		pVertex[9].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);
		pVertex[10].vPosition	= _vec3( 0.5f,	-0.5f,	 0.5f);
		pVertex[11].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);

		pVertex[12].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);	//전면
		pVertex[13].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);
		pVertex[14].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);
		pVertex[15].vPosition	= _vec3( 0.5f,	-0.5f,	 0.5f);

		pVertex[16].vPosition	= _vec3(-0.5f,	 0.5f,	 0.5f);	//상단면
		pVertex[17].vPosition	= _vec3( 0.5f,	 0.5f,	 0.5f);
		pVertex[18].vPosition	= _vec3( 0.5f,	 0.5f,	-0.5f);
		pVertex[19].vPosition	= _vec3(-0.5f,	 0.5f,	-0.5f);

		pVertex[20].vPosition	= _vec3(-0.5f,	-0.5f,	-0.5f);	//하단면
		pVertex[21].vPosition	= _vec3( 0.5f,	-0.5f,	-0.5f);
		pVertex[22].vPosition	= _vec3( 0.5f,	-0.5f,	 0.5f);
		pVertex[23].vPosition	= _vec3(-0.5f,	-0.5f,	 0.5f);

#define NORMAL(__START_INDEX, __NORMAL)			\
	pVertex[__START_INDEX+0].vNormal = __NORMAL;\
	pVertex[__START_INDEX+1].vNormal = __NORMAL;\
	pVertex[__START_INDEX+2].vNormal = __NORMAL;\
	pVertex[__START_INDEX+3].vNormal = __NORMAL

		NORMAL(0,	_vec3(0.f, 0.f, -1.f));
		NORMAL(4,	_vec3(-1.f, 0.f, 0.f));
		NORMAL(8,	_vec3(1.f, 0.f, 0.f));
		NORMAL(12,	_vec3(0.f, 0.f, 1.f));
		NORMAL(16,	_vec3(0.f, 1.f, 0.f));
		NORMAL(20,	_vec3(0.f, -1.f, 0.f));

#undef NORMAL
		
		for(int i = 0; i < 6; ++i) {
			int Index = i*4;
			pVertex[Index].vUV		= _vec2(0.f, 0.f);
			pVertex[Index+1].vUV	= _vec2(1.f, 0.f);
			pVertex[Index+2].vUV	= _vec2(1.f, 1.f);
			pVertex[Index+3].vUV	= _vec2(0.f, 1.f);
		}
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		IndexBufferInitializer(pIndex, m_iIBCnt,
			0, 1, 2,		0, 2, 3,
			4, 5, 6,		4, 6, 7,
			8, 9, 10,		8, 10,11,
			12,13,14,		12,14,15,
			16,17,18,		16,18,19,
			20,21,22,		20,22,23);
	}

	m_pIB->Unlock();

	return S_OK;
}