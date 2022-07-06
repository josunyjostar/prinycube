#ifndef Terrain_h__
#define Terrain_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain : public CVIBuffer
{
protected:
	explicit CTerrain();
	virtual ~CTerrain();

public:
	DeclRTTI(CTerrain, CVIBuffer);
	friend CComponent;

	void SetPlaneSize(_uint _iX, _uint _iY);
	void SetGrid(_float _fGridX, _float _fGridY);
	void SetTerrain(_uint _iX, _uint _iY, _float _fGridX, _float _fGridY);
	void SetHeightTexture(_char* _pHeightTexturePath, _bool _bAlphaTexture, _float _fMaxHeight);
	bool GetHeight(_float& _fHeight, _vec3 _vPos);
	bool GetRayColPos(_vec3& _vColPos, _ray _rCamRay);

public:
	virtual HRESULT	CreateBuffer(const CVIBuffer* rhs = NULL);

protected:
	_vec3*	m_pVertex;
	_uint	m_iX;
	_uint	m_iY;
	_float	m_fGridX;
	_float	m_fGridY;

private:
	void IntersectTri(const _vec3& _vVtx0, const _vec3& _vVtx1, const _vec3& _vVtx2, const _ray& _rCamRay, bool& _bCol, float& _fMinDist, _vec3& _vMinColPos);
};

END

TYPEDEF_RCP(Engine::CTerrain, TERRAIN);

#endif // Terrain_h__
