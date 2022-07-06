#include "Terrain.h"
#include <atlimage.h>
using namespace Engine;

CTerrain::CTerrain()
:m_pVertex(NULL)
,m_iX(1)
,m_iY(1)
,m_fGridX(1.f)
,m_fGridY(1.f)
{
	m_Matrial.Ambient	= D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

CTerrain::~CTerrain()
{
	Safe_Delete(m_pVertex);
}

void CTerrain::SetPlaneSize(_uint _iX, _uint _iY)
{
	if(_iX == 0 || _iY == 0) return;

	m_iX = _iX;
	m_iY = _iY;

	this->Release_VIBuffer();
	this->CreateBuffer();
}

void CTerrain::SetGrid(_float _fGridX, _float _fGridY)
{
	if(_fGridX <= 0.f || _fGridY <= 0.f) return;

	m_fGridX = _fGridX;
	m_fGridY = _fGridY;

	VTXNORMALTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	float LTX = -(m_iX * 0.5f);
	float LTY = m_iY * 0.5f;
	int xvSize = m_iX+1;
	int yvSize = m_iY+1;
	int maxvSize = xvSize*yvSize;

	for(int i = 0; i < maxvSize; ++i)
	{
		float x = float(i%xvSize);
		float y = float(i/xvSize);

		pVertex[i].vUV = _vec2(x / float(m_iX) * m_fGridX, y / float(m_iY) * m_fGridY);
	}				

	m_pVB->Unlock();
}

void CTerrain::SetTerrain(_uint _iX, _uint _iY, _float _fGridX, _float _fGridY)
{
	if(_iX == 0 || _iY == 0 || _fGridX <= 0.f || _fGridY <= 0.f) return;

	m_iX = _iX;
	m_iY = _iY;
	m_fGridX = _fGridX;
	m_fGridY = _fGridY;

	this->Release_VIBuffer();
	this->CreateBuffer();
}

void CTerrain::SetHeightTexture(_char* _pHeightTexturePath, _bool _bAlphaTexture, _float _fMaxHeight)
{
	if(!m_pVertex) return;

	int xvSize = m_iX+1;
	int yvSize = m_iY+1;
	int maxvSize = xvSize*yvSize;
	int maxSize = m_iX * m_iY;

	CImage Image;
	CString strImagePath = _pHeightTexturePath;
	HRESULT hResult = Image.Load(strImagePath);

	if (FAILED(hResult)) return;

	int ImgWidth = Image.GetWidth();
	int ImgHeight = Image.GetHeight();
	int pitch = Image.GetPitch();
	BYTE* pRawData = (BYTE*)Image.GetBits() + pitch * (ImgHeight - 1);
	int iColorBits = (_bAlphaTexture)?4:3;

	VTXNORMALTEX* pVertex;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	float LTX = m_iX * -0.5f;
	float LTY = m_iY * 0.5f;

	for(int i = 0; i < maxvSize; ++i)
	{
		float x = float(i%xvSize);
		float y = float(i/xvSize);

		int iImageX = int(float(x) / float(xvSize) * float(ImgWidth));
		int iImageY = int(float(y) / float(yvSize) * float(ImgHeight));
		int iImageIndex = (iImageX + iImageY * ImgHeight) * iColorBits;	//Read blue color

		pVertex[i].vPosition = _vec3(LTX + x, m_pVertex[i].y = pRawData[iImageIndex] / 255.f * _fMaxHeight, LTY - y);
		pVertex[i].vNormal = _vec3(0.f, 0.f, 0.f);
	}

	for(int i = 0; i < maxSize; ++i)
	{
		WORD LT = i%m_iX+i/m_iX*xvSize;
		WORD RT = LT+1;
		WORD LB = LT+xvSize;
		WORD RB = RT+xvSize;

		_vec3 vNormal = PolygonNormal(pVertex[LT].vPosition, pVertex[RT].vPosition, pVertex[RB].vPosition);

		pVertex[LT].vNormal += vNormal;
		pVertex[RT].vNormal += vNormal;
		pVertex[RB].vNormal += vNormal;

		vNormal = PolygonNormal(pVertex[LT].vPosition, pVertex[RB].vPosition, pVertex[LB].vPosition);

		pVertex[LT].vNormal += vNormal;
		pVertex[RB].vNormal += vNormal;
		pVertex[LB].vNormal += vNormal;
	}

	for(int i = 0; i < maxvSize; ++i)
	{
		pVertex[i].vNormal = Normalize(pVertex[i].vNormal);
	}

	m_pVB->Unlock();
}

bool CTerrain::GetHeight(_float& _fHeight, _vec3 _vPos)
{
	_float fLTX = m_iX * -0.5f;
	_float fLTY = m_iY * 0.5f;

	if(fLTX > _vPos.x || -fLTX < _vPos.x || fLTY < _vPos.z || -fLTY > _vPos.z) return false;

	_float fX = _vPos.x - fLTX;
	_float fY = fLTY - _vPos.z;
	_uint iX = (_uint)fX;
	_uint iY = (_uint)fY;
	fX -= iX;
	fY -= iY;

	WORD LT = iX+iY*(m_iX+1);
	WORD RT = LT+1;
	WORD LB = LT+(m_iX+1);
	WORD RB = RT+(m_iX+1);

	if(fX > fY)	_fHeight = m_pVertex[LT].y + fX*(m_pVertex[RT].y-m_pVertex[LT].y) + fY*(m_pVertex[RB].y-m_pVertex[RT].y);
	else		_fHeight = m_pVertex[LT].y + fX*(m_pVertex[RB].y-m_pVertex[LB].y) + fY*(m_pVertex[LB].y-m_pVertex[LT].y);
	
	return true;
}

bool CTerrain::GetRayColPos(_vec3& _vColPos, _ray _rCamRay)
{
	int maxSize = m_iX * m_iY;
	int xvSize = m_iX+1;

	bool bCol = false;
	float fMinDist;
	_vec3 vMinColPos;

	for(int i = 0; i < maxSize; ++i)
	{
		WORD LT = i%m_iX+i/m_iX*xvSize;
		WORD RT = LT+1;
		WORD LB = LT+xvSize;
		WORD RB = RT+xvSize;

		IntersectTri(m_pVertex[RT], m_pVertex[RB], m_pVertex[LT], _rCamRay, bCol, fMinDist, vMinColPos);
		IntersectTri(m_pVertex[LB], m_pVertex[LT], m_pVertex[RB], _rCamRay, bCol, fMinDist, vMinColPos);
	}

	if(bCol) _vColPos = vMinColPos;

	return bCol;
}

HRESULT CTerrain::CreateBuffer(const CVIBuffer* rhs /*= NULL*/)
{
	int xvSize = m_iX+1;
	int yvSize = m_iY+1;
	int maxvSize = xvSize*yvSize;
	int maxSize = m_iX * m_iY;

	m_dwFVF		= D3DFVF_VTXNORMALTEX;
	m_dwVBSize	= sizeof(VTXNORMALTEX);
	m_iVBCnt	= maxvSize;

	m_dwIBSize	= sizeof(WORD);
	m_iIBCnt	= maxSize*6;

	if(FAILED(this->Create_VIBuffer()))
		return E_FAIL;


	Safe_Delete(m_pVertex);
	m_pVertex = new _vec3[maxvSize];

	VTXNORMALTEX* pVertex;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	if(rhs) {
		memcpy(m_pVertex, ((CTerrain*)rhs)->m_pVertex, sizeof(_vec3)*maxvSize);
		this->VBCopy((void**)&pVertex, rhs);
	}
	else
	{
		float LTX = -(m_iX * 0.5f);
		float LTY = m_iY * 0.5f;

		for(int i = 0; i < maxvSize; ++i)
		{
			float x = float(i%xvSize);
			float y = float(i/xvSize);

			pVertex[i].vPosition = _vec3(LTX + x, 0.f, LTY - y);
			m_pVertex[i] = pVertex[i].vPosition;
			pVertex[i].vNormal = _vec3(0.f, 1.f, 0.f);
			pVertex[i].vUV = _vec2(x / float(m_iX) * m_fGridX, y / float(m_iY) * m_fGridY);
		}
	}
	m_pVB->Unlock();



	WORD* pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if(rhs) this->IBCopy((void**)&pIndex, rhs);
	else
	{
		int maxSize = m_iX * m_iY;

		for(int i = 0; i < maxSize; ++i)
		{
			WORD LT = i%m_iX+i/m_iX*xvSize;
			WORD RT = LT+1;
			WORD LB = LT+xvSize;
			WORD RB = RT+xvSize;
			int Index = i*6;

			pIndex[Index]	= LT;
			pIndex[Index+1] = RT;
			pIndex[Index+2] = RB;

			pIndex[Index+3] = LT;
			pIndex[Index+4] = RB;
			pIndex[Index+5] = LB;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CTerrain::IntersectTri(const _vec3& _vVtx0, const _vec3& _vVtx1, const _vec3& _vVtx2, const _ray& _rCamRay, 
							bool& _bCol, float& _fMinDist, _vec3& _vMinColPos)
{
	float fDist = 0.f;
	if(D3DXIntersectTri(&_vVtx0, &_vVtx1, &_vVtx2,	&_rCamRay.Pos, &_rCamRay.Dir, NULL, NULL, &fDist))
	{
		if(!_bCol) {
			_bCol = true;
			_fMinDist = fDist;
			_vMinColPos = _rCamRay.Pos + _rCamRay.Dir * fDist;
		}
		else if(fDist < _fMinDist)
		{
			_fMinDist = fDist;
			_vMinColPos = _rCamRay.Pos + _rCamRay.Dir * fDist;
		}
	}
}