#include "stdafx.h"
#include "Cylinder_Texture.h"
using namespace Engine;

CCylinder_Texture::CCylinder_Texture()
{
	m_Matrial.Ambient	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

CCylinder_Texture::~CCylinder_Texture()
{
}

HRESULT CCylinder_Texture::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	int Edge = 12;

	m_dwFVF		= D3DFVF_VTXNORMALTEX;
	m_dwVBSize	= sizeof(Engine::VTXNORMALTEX);
	m_iVBCnt	= Edge * 4 + Edge * 2;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= Edge * 6 + (Edge-2)*6;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;



    Engine::VTXNORMALTEX* pVertex = NULL;
	m_pVB->Lock( 0, 0, ( void** )&pVertex, 0 );
	
	int Edgetemp = Edge*4;

	if(rhs) this->VBCopy((void**)&pVertex, rhs);
	else
	{
		FLOAT theta;
		FLOAT thetaPlus;
		for( int i = 0; i < Edge; ++i )
		{
			theta = ( 2 * D3DX_PI * i ) / ( Edge );
			thetaPlus = ( 2 * D3DX_PI * (i+1) ) / ( Edge );

			pVertex[4 * i + 0].vPosition = D3DXVECTOR3( sinf( theta )*0.5f,  0.5f, cosf( theta ) *-0.5f);
			pVertex[4 * i + 0].vNormal;
			pVertex[4 * i + 0].vUV.x = ( ( FLOAT )i ) / ( Edge );
			pVertex[4 * i + 0].vUV.y = 0.0f;

			pVertex[4 * i + 1].vPosition = D3DXVECTOR3( sinf( thetaPlus )*0.5f,  0.5f, cosf( thetaPlus ) *-0.5f);
			pVertex[4 * i + 1].vNormal;
			pVertex[4 * i + 1].vUV.x = ( ( FLOAT )(i+1) ) / ( Edge );
			pVertex[4 * i + 1].vUV.y = 0.0f;
			
			pVertex[4 * i + 2].vPosition = D3DXVECTOR3( sinf( thetaPlus )*0.5f, -0.5f, cosf( thetaPlus )*-0.5f );
			pVertex[4 * i + 2].vNormal;
			pVertex[4 * i + 2].vUV.x = ( ( FLOAT )(i+1) ) / ( Edge );
			pVertex[4 * i + 2].vUV.y = 1.0f;

			pVertex[4 * i + 3].vPosition = D3DXVECTOR3( sinf( theta )*0.5f, -0.5f, cosf( theta ) *-0.5f);
			pVertex[4 * i + 3].vNormal;
			pVertex[4 * i + 3].vUV.x = ( ( FLOAT )i ) / ( Edge );
			pVertex[4 * i + 3].vUV.y = 1.0f;
		}
		for( int i = Edgetemp; i < Edgetemp+Edge; ++i ) // 상
		{
			int Ri = i-Edgetemp;
			pVertex[i]= pVertex[Ri*4];
		}	
		for( int i = Edgetemp+Edge; i < Edgetemp+Edge+Edge; ++i ) // 상
		{
			int Ri = i-Edgetemp-Edge;
			pVertex[i]= pVertex[Ri*4+3];
		}


#define NORMAL(__START_INDEX, __NORMAL)			\
		pVertex[__START_INDEX+0].vNormal = __NORMAL;\
		pVertex[__START_INDEX+1].vNormal = __NORMAL;\
		pVertex[__START_INDEX+2].vNormal = __NORMAL;\
		pVertex[__START_INDEX+3].vNormal = __NORMAL;

		_vec3 vCross;
#define CROSS(__START_INDEX)			\
		D3DXVec3Cross(&vCross, &(pVertex[__START_INDEX+1].vPosition - pVertex[__START_INDEX+0].vPosition), &(pVertex[__START_INDEX+2].vPosition - pVertex[__START_INDEX+1].vPosition)); \
		D3DXVec3Normalize(&vCross, &vCross);

		for( int i = 0; i < Edge; ++i )
		{
			CROSS(i*4);
			NORMAL(i*4,	vCross);
		}
		for( int i = Edgetemp; i < Edgetemp+Edge; ++i ) // 상
		{
			pVertex[i].vNormal = _vec3(0.f, 1.f, 0.f);
		}	
		for( int i = Edgetemp+Edge; i < Edgetemp+Edge+Edge; ++i ) // 하
		{
			pVertex[i].vNormal = _vec3(0.f, -1.f, 0.f);
		}
#undef CROSS
#undef NORMAL	
	}
	m_pVB->Unlock();

	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		Edgetemp = Edge*6;
		for( int i = 0; i < Edge; ++i)
		{
			int i6 = i*6;
			int i4 = i*4;
			pIndex[i6+0] = i4;
			pIndex[i6+1] = i4+1;
			pIndex[i6+2] = i4+2;
			pIndex[i6+3] = i4;
			pIndex[i6+4] = i4+2;
			pIndex[i6+5] = i4+3;
		}
		for( int i = Edge; i < Edge + (Edge-2); ++i ) // 상
		{
			int Ri = i-Edge;
			int i6 = Edge * 6 + Ri*3;
			int i4 = i*4;
			pIndex[i6+0] = Edge*4+Ri+2;
			pIndex[i6+1] = Edge*4+Ri+1;
			pIndex[i6+2] = Edge*4;
		}	
		for( int i = Edge + (Edge-2); i < Edge + (Edge-2) + (Edge-2); ++i ) // 하
		{
			int Ri = i-Edge - Edge+2;
			int i6 = Edge * 6 + Ri*3 + (Edge-2)*3;
			int i4 = i*4;
			int EEdge = Edge + (Edge-2);
			pIndex[i6+0] = Edge*4+(Edge);
			pIndex[i6+1] = Edge*4+Ri+1+(Edge);
			pIndex[i6+2] = Edge*4+Ri+2+(Edge);
		}	
	}
	m_pIB->Unlock();

	return S_OK;
}