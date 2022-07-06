// Struct Method
//////////////////////////////////////////////////////////////////////////
// Struct Function Index
/*
*/
//////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 operator*(const D3DXVECTOR3& _vDest, const D3DXVECTOR3& _vSour)
{
	return D3DXVECTOR3(_vSour.x*_vDest.x, _vSour.y*_vDest.y, _vSour.z*_vDest.z);
}

D3DXVECTOR4 operator*(const D3DXVECTOR4& _vDest, const D3DXVECTOR4& _vSour)
{
	return D3DXVECTOR4(_vSour.x*_vDest.x, _vSour.y*_vDest.y, _vSour.z*_vDest.z, _vSour.w*_vDest.w);
}

//////////////////////////////////////////////////////////////////////////