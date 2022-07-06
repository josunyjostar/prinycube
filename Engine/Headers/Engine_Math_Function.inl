// Inline Function
//////////////////////////////////////////////////////////////////////////
// Function Index
/*
float V2R(const _vec2& _Vector)
float V2D(const _vec2& _Vector)
_vec2 R2V(float _Radian)
_vec2 D2V(float _Degree)
void Swap(T& a, T& b)
_float Lerp(const _float& _vFrom, const _float& _vTo, float _fTime)
enum TWEEN_TYPE { TWEEN_LINEAR, TWEEN_IN, TWEEN_OUT, TWEEN_INOUT }
float TweenVal(float _fT, float _fS, float _fE, TWEEN_TYPE _eType, float _pow)

//_vec2
float Dot(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
float Cross(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
float sqrMagnitude(const D3DXVECTOR2& _Vector)	
float Magnitude(const D3DXVECTOR2& _Vector)
float Distance(const D3DXVECTOR2& _Pos1, const D3DXVECTOR2& _Pos2)
D3DXVECTOR2 GetVector(const D3DXVECTOR2& _from, const D3DXVECTOR2& _to)
D3DXVECTOR2 LookAt(const D3DXVECTOR2& _from, const D3DXVECTOR2& _to)
float V2ToR(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
float V2ToD(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
D3DXVECTOR2 Normalize(const D3DXVECTOR2& _Vector)
D3DXVECTOR2 Rotate(const D3DXVECTOR2& _Base, float _fRadian)

//_vec3
float Dot(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
float Dot(const D3DXVECTOR3& _Vector)
float Cross(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
float sqrMagnitude(const D3DXVECTOR3& _Vector)	
float Magnitude(const D3DXVECTOR3& _Vector)
float Distance(const D3DXVECTOR3& _Pos1, const D3DXVECTOR3& _Pos2)
D3DXVECTOR3 GetVector(const D3DXVECTOR3& _from, const D3DXVECTOR3& _to)
D3DXVECTOR3 LookAt(const D3DXVECTOR3& _from, const D3DXVECTOR3& _to)
float V2ToR(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
float V2ToD(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
D3DXVECTOR3 Normalize(const D3DXVECTOR3& _Vector)
D3DXVECTOR3 TransformNormal(const D3DXVECTOR3& _Vec, const D3DXMATRIX& _mat)
D3DXVECTOR3 TransformCoord(const D3DXVECTOR3& _Vec, const D3DXMATRIX& _mat)
D3DXVECTOR3 PolygonNormal(const D3DXVECTOR3& _Vtx1, const D3DXVECTOR3& _Vtx2, const D3DXVECTOR3& _Vtx3)
D3DXVECTOR3 Lerp(const D3DXVECTOR3& _vFrom, const D3DXVECTOR3& _vTo, float _fTime)
D3DXVECTOR3 Slerp(const D3DXVECTOR3& _vFrom, const D3DXVECTOR3& _vTo, float _fTime)
D3DXMATRIX Slerp(const D3DXMATRIX& _vFrom, const D3DXMATRIX& _vTo, float _fTime)
D3DXMATRIX MoveMatrix(const D3DXMATRIX& _mat, const D3DXVECTOR3& _vPos)

//Collision Detected
bool SphereInSphere(const D3DXVECTOR3& _vCenter1, float _fRadius1, const D3DXVECTOR3& _vCenter2, float _fRadius2)
bool SphereInRay(const D3DXVECTOR3& _vCenter, float _fRadius, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir, D3DXVECTOR3* _vColPos = NULL)
bool SphereInBox(const D3DXVECTOR3& _vCenter, float _fRadius, const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize)
bool SphereInBox(const D3DXVECTOR3& _vCenter, float _fRadius, const D3DXMATRIX& _matBoxWorld, const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize)
bool BoxInBox(const D3DXVECTOR3& _vBoxPos1, const D3DXVECTOR3& _vBoxSize1, const D3DXVECTOR3& _vBoxPos2, const D3DXVECTOR3& _vBoxSize2)
bool BoxInBox(const D3DXMATRIX& _matBoxWorld1, const D3DXVECTOR3& _vBoxPos1, const D3DXVECTOR3& _vBoxSize1, 
			const D3DXMATRIX& _matBoxWorld2, const D3DXVECTOR3& _vBoxPos2, const D3DXVECTOR3& _vBoxSize2)
bool BoxInRay(const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir, D3DXVECTOR3* _vColPos = NULL)
bool BoxInRay(const D3DXMATRIX& _matBoxWorld, const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir, D3DXVECTOR3* _vColPos = NULL)
*/
//////////////////////////////////////////////////////////////////////////

inline float V2R(const _vec2& _Vector)
{
	return atan2(_Vector.y, _Vector.x);
}

inline float V2D(const _vec2& _Vector)
{
	return R2D(V2R(_Vector));
}

inline _vec2 R2V(float _Radian)
{
	return _vec2(cosf(_Radian), sinf(_Radian));
}

inline _vec2 D2V(float _Degree)
{
	return R2V(D2R(_Degree)); 
}

template<typename T>
inline void Swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

inline float Lerp(const float& _fFrom, const float& _fTo, float _fTime)
{
	return _fFrom+(_fTo-_fFrom)*_fTime;
}

enum TWEEN_TYPE { TWEEN_LINEAR, TWEEN_IN, TWEEN_OUT, TWEEN_INOUT };
inline float TweenVal(float _fT, float _fS, float _fE, TWEEN_TYPE _eType, float _pow)
{
	float f;
	switch(_eType)
	{
	case TWEEN_LINEAR:
		return _fS + (_fE - _fS) * _fT;

	case TWEEN_IN:
		f = tanf(_fT*PI*0.25f);
		f = pow(f,_pow);
		return _fS + (_fE - _fS) * f;

	case TWEEN_OUT:
		f = tanf(PI*0.25f*(_fT-1.f))+1.f;
		f = pow(f,_pow);
		return _fS + (_fE - _fS) * f;

	case TWEEN_INOUT:
		f = (sinf(PI*(_fT-0.5f))+1.f) / 2.f;
		f = pow(f,_pow);
		return _fS + (_fE - _fS) * f;

	default:
		return 0.f;
	}
}

// _vec2
inline float Dot(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
{
	return _Vector1.x*_Vector2.x + _Vector1.y*_Vector2.y;
}

inline float Cross(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
{
	return _Vector1.x*_Vector2.y - _Vector1.y*_Vector2.x;
}

inline float sqrMagnitude(const D3DXVECTOR2& _Vector)	
{ 
	return SQR(_Vector.x) + SQR(_Vector.y); 
}

inline float Magnitude(const D3DXVECTOR2& _Vector)
{ 
	return sqrtf(sqrMagnitude(_Vector)); 
}

inline float sqrDistance(const D3DXVECTOR2& _Pos1, const D3DXVECTOR2& _Pos2)	
{
	return sqrMagnitude(_Pos2-_Pos1);
}

inline float Distance(const D3DXVECTOR2& _Pos1, const D3DXVECTOR2& _Pos2)
{
	return sqrtf(sqrDistance(_Pos1, _Pos2)); 
}

inline D3DXVECTOR2 GetVector(const D3DXVECTOR2& _from, const D3DXVECTOR2& _to)
{
	return _to-_from;
}

inline D3DXVECTOR2 LookAt(const D3DXVECTOR2& _from, const D3DXVECTOR2& _to)
{
	float f = Distance(_from, _to);
	if(f == 0.f) 
		return D3DXVECTOR2(0.f, 0.f);
	else 
		return (_to-_from)/f;
}

inline float V2ToR(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
{
	return acosf(Dot(_Vector1, _Vector2));
}

inline float V2ToD(const D3DXVECTOR2& _Vector1, const D3DXVECTOR2& _Vector2)
{
	return R2D(acosf(Dot(_Vector1, _Vector2)));
}

inline D3DXVECTOR2 Normalize(const D3DXVECTOR2& _Vector)
{
	return _Vector/Magnitude(_Vector);
}

inline D3DXVECTOR2 Rotate(const D3DXVECTOR2& _Base, float _fRadian)
{
	float r = atan2(_Base.x, _Base.y) + _fRadian;
	return D3DXVECTOR2(cosf(r), sinf(r)) * Magnitude(_Base);
}


// _vec3
inline float Dot(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
{
	return _Vector1.x*_Vector2.x + _Vector1.y*_Vector2.y + _Vector1.z*_Vector2.z;
}

inline float Dot(const D3DXVECTOR3& _Vector)
{
	return SQR(_Vector.x) + SQR(_Vector.y) + SQR(_Vector.z);
}

inline _vec3 Cross(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
{
	return _vec3(_Vector1.y*_Vector2.z - _Vector1.z*_Vector2.y
		, _Vector1.z*_Vector2.x - _Vector1.x*_Vector2.z
		, _Vector1.x*_Vector2.y - _Vector1.y*_Vector2.x);
}

inline float sqrMagnitude(const D3DXVECTOR3& _Vector)	
{ 
	return SQR(_Vector.x) + SQR(_Vector.y) + SQR(_Vector.z);
}

inline float Magnitude(const D3DXVECTOR3& _Vector)
{ 
	return sqrtf(sqrMagnitude(_Vector)); 
}

inline float sqrDistance(const D3DXVECTOR3& _Pos1, const D3DXVECTOR3& _Pos2)	
{
	return sqrMagnitude(_Pos2-_Pos1);
}

inline float Distance(const D3DXVECTOR3& _Pos1, const D3DXVECTOR3& _Pos2)
{
	return sqrtf(sqrDistance(_Pos1, _Pos2)); 
}

inline D3DXVECTOR3 GetVector(const D3DXVECTOR3& _from, const D3DXVECTOR3& _to)
{
	return _to-_from;
}

inline D3DXVECTOR3 LookAt(const D3DXVECTOR3& _from, const D3DXVECTOR3& _to)
{
	float f = Distance(_from, _to);
	if(f == 0.f) 
		return D3DXVECTOR3(0.f, 0.f, 0.f);
	else 
		return (_to-_from)/f;
}

inline float V2ToR(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
{
	return acosf(Dot(_Vector1, _Vector2));
}

inline float V2ToD(const D3DXVECTOR3& _Vector1, const D3DXVECTOR3& _Vector2)
{
	return R2D(acosf(Dot(_Vector1, _Vector2)));
}

inline D3DXVECTOR3 Normalize(const D3DXVECTOR3& _Vector)
{
	float fMagnitude = Magnitude(_Vector);
	return (fMagnitude)?_Vector/Magnitude(_Vector):_vec3();
}

inline D3DXVECTOR3 TransformNormal(const D3DXVECTOR3& _Vec, const D3DXMATRIX& _mat)
{
	return D3DXVECTOR3
		(_Vec.x*_mat._11 + _Vec.y*_mat._21 + _Vec.z*_mat._31
		,_Vec.x*_mat._12 + _Vec.y*_mat._22 + _Vec.z*_mat._32
		,_Vec.x*_mat._13 + _Vec.y*_mat._23 + _Vec.z*_mat._33);
}

inline D3DXVECTOR3 TransformCoord(const D3DXVECTOR3& _Vec, const D3DXMATRIX& _mat)
{
	return D3DXVECTOR3
		(_Vec.x*_mat._11 + _Vec.y*_mat._21 + _Vec.z*_mat._31 + _mat._41
		,_Vec.x*_mat._12 + _Vec.y*_mat._22 + _Vec.z*_mat._32 + _mat._42
		,_Vec.x*_mat._13 + _Vec.y*_mat._23 + _Vec.z*_mat._33 + _mat._43);
}

inline D3DXVECTOR3 PolygonNormal(const D3DXVECTOR3& _Vtx1, const D3DXVECTOR3& _Vtx2, const D3DXVECTOR3& _Vtx3)
{
	return Normalize(Cross(_Vtx2 - _Vtx1, _Vtx3 - _Vtx2));
}

inline D3DXVECTOR3 Lerp(const D3DXVECTOR3& _vFrom, const D3DXVECTOR3& _vTo, float _fTime)
{
	return _vFrom+(_vTo-_vFrom)*_fTime;
}

inline D3DXVECTOR3 Slerp(const D3DXVECTOR3& _vFrom, const D3DXVECTOR3& _vTo, float _fTime)
{
	float fDot = Dot(Normalize(_vFrom), Normalize(_vTo));
	if(fDot >= CUTONE) return _vTo;

	float fAngle = acosf(fDot) * _fTime;

	D3DXVECTOR3 a;
	if(fDot <= -CUTONE)
	{
		D3DXVECTOR3 vUp(0.f, 1.f, 0.f);
		D3DXVECTOR3 vRight = Cross(vUp, _vFrom);

		if(sqrMagnitude(vRight) >= CUTZERO)
			a = Normalize(Cross(vRight, _vFrom));
		else {
			vRight = D3DXVECTOR3(1.f, 0.f, 0.f);
			a = Normalize(Cross(_vFrom, vRight));
		}
	}
	else a = Normalize(Cross(_vTo, _vFrom));

	float c = cosf(fAngle);
	float s = sinf(fAngle);

	return _vec3(Dot(_vFrom, _vec3(SQR(a.x)+(1.f-SQR(a.x))*c, a.x*a.y*(1.f-c)+a.x*s, a.x*a.z*(1.f-c)-a.y*s))
		,Dot(_vFrom, _vec3(a.x*a.y*(1.f-c)-a.z*s, SQR(a.y)+(1.f-SQR(a.y))*c, a.y*a.z*(1.f-c)+a.x*s))
		,Dot(_vFrom, _vec3(a.x*a.z*(1.f-c)+a.y*s, a.y*a.z*(1.f-c)-a.x*s, SQR(a.z)+(1.f-SQR(a.z))*c)));
}

inline D3DXMATRIX Slerp(const D3DXMATRIX& _matFrom, const D3DXMATRIX& _matTo, float _fTime)
{
	float fDot;
	float fMinDot = CUTONE;
	int iMinIndex;
	bool bCutOut = true;
	for(int i = 0; i < 3; ++i)
	{
		fDot = Dot(Normalize(*(D3DXVECTOR3*)_matFrom.m[i]), Normalize(*(D3DXVECTOR3*)_matTo.m[i]));

		if(fDot < fMinDot) {
			fMinDot = fDot;
			iMinIndex = i;
			bCutOut = false;
		}
	}
	if(bCutOut) return _matTo;

	_vec3 vFrom = *(D3DXVECTOR3*)_matFrom.m[iMinIndex];
	_vec3 vTo = *(D3DXVECTOR3*)_matTo.m[iMinIndex];
	fDot = fMinDot;
	
	if(fDot >= CUTONE) return _matTo;

	float fAngle = acosf(fDot) * _fTime;

	D3DXVECTOR3 a;
	if(fDot <= -CUTONE)
	{
		D3DXVECTOR3 vUp(0.f, 1.f, 0.f);
		D3DXVECTOR3 vRight = Cross(vUp, vFrom);

		if(sqrMagnitude(vRight) >= CUTZERO)
			a = Normalize(Cross(vRight, vFrom));
		else {
			vRight = D3DXVECTOR3(1.f, 0.f, 0.f);
			a = Normalize(Cross(vFrom, vRight));
		}
	}
	else a = Normalize(Cross(vTo, vFrom));

	float c = cosf(fAngle);
	float s = sinf(fAngle);

	D3DXMATRIX mat = _matFrom;
	for(int i = 0; i < 3; ++i) {
		_vec3 vFromAxis = *(D3DXVECTOR3*)_matFrom.m[i];
		memcpy(mat.m[i], &_vec3(Dot(vFromAxis, _vec3(SQR(a.x)+(1.f-SQR(a.x))*c, a.x*a.y*(1.f-c)+a.x*s, a.x*a.z*(1.f-c)-a.y*s))
			,Dot(vFromAxis, _vec3(a.x*a.y*(1.f-c)-a.z*s, SQR(a.y)+(1.f-SQR(a.y))*c, a.y*a.z*(1.f-c)+a.x*s))
			,Dot(vFromAxis, _vec3(a.x*a.z*(1.f-c)+a.y*s, a.y*a.z*(1.f-c)-a.x*s, SQR(a.z)+(1.f-SQR(a.z))*c))), sizeof(_vec3));
	}
	return mat;
}

inline D3DXMATRIX MoveMatrix(const D3DXMATRIX& _mat, const D3DXVECTOR3& _vPos)
{
	D3DXMATRIX mat = _mat;
	memcpy(mat.m[3], &(*(D3DXVECTOR3*)mat.m[3] + _vPos), sizeof(D3DXVECTOR3));
	return mat;
}

//Collision
inline bool SphereInSphere(const D3DXVECTOR3& _vCenter1, float _fRadius1, const D3DXVECTOR3& _vCenter2, float _fRadius2)
{
	_vec3 vCenterDiff = _vCenter1 - _vCenter2;
	float fRadiusSum = _fRadius1 + _fRadius2;
	return (Dot(vCenterDiff) <= SQR(fRadiusSum));
}

inline bool SphereInRay(const D3DXVECTOR3& _vCenter, float _fRadius, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir, D3DXVECTOR3* _vColPos = NULL)
{
	_vec3 vSToR = _vCenter-_vRayPos;

	float fWsq = Dot(vSToR);
	float fProj = Dot(vSToR, _vRayDir);

	if( fProj < 0.f && fWsq > SQR(_fRadius) ) return false;

	float fVsq = Dot(_vRayDir);

	if(!((fVsq*fWsq - SQR(fProj)) <= (fVsq * SQR(_fRadius)))) return false;

	if(_vColPos) {
		float d = sqrtf(fWsq-SQR(fProj));
		*_vColPos = _vRayPos + _vRayDir * (fProj - sqrtf(SQR(_fRadius) - SQR(d)));

		if(sqrDistance(_vRayPos, _vCenter) <= SQR(_fRadius))
			*_vColPos = _vRayPos + _vRayDir * (fProj + sqrtf(SQR(_fRadius) - SQR(d)));
		else
			*_vColPos = _vRayPos + _vRayDir * (fProj - sqrtf(SQR(_fRadius) - SQR(d)));
	}
	return true;
}

inline bool SphereInBox(const D3DXVECTOR3& _vCenter, float _fRadius, const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize)
{
	float fSqrDist = 0.f;
	_vec3 vBounds[2];

	vBounds[0] = _vBoxPos - _vBoxSize * 0.5f;
	vBounds[1] = _vBoxPos + _vBoxSize * 0.5f;
	
	for(int i = 0; i < 3; ++i) {
		if ( _vCenter[i] < vBounds[0][i] ) {
			float fVal = (vBounds[0][i] - _vCenter[i]);
			fSqrDist += SQR(fVal);
		}
		if ( _vCenter[i] > vBounds[1][i] ) {
			float fVal = (_vCenter[i] - vBounds[1][i]);
			fSqrDist += SQR(fVal);
		}
	}

	return (fSqrDist <= SQR(_fRadius));
}

inline bool SphereInBox(const D3DXVECTOR3& _vCenter, float _fRadius, const D3DXMATRIX& _matBoxWorld, const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize)
{
	float fSqrDist = 0.f;
	_vec3 vBounds[2];

	vBounds[0] = _vBoxPos - _vBoxSize * 0.5f;
	vBounds[1] = _vBoxPos + _vBoxSize * 0.5f;

	_matrix matInvBoxWorld;
	D3DXMatrixInverse(&matInvBoxWorld, NULL, &_matBoxWorld);
	_vec3 vLocalCenter = TransformCoord(_vCenter, matInvBoxWorld);

	for(int i = 0; i < 3; ++i) {
		if ( vLocalCenter[i] < vBounds[0][i] ) {
			float fVal = (vBounds[0][i] - vLocalCenter[i]);
			fSqrDist += SQR(fVal);
		}
		if ( vLocalCenter[i] > vBounds[1][i] ) {
			float fVal = (vLocalCenter[i] - vBounds[1][i]);
			fSqrDist += SQR(fVal);
		}
	}

	return (fSqrDist <= SQR(_fRadius));
}

inline bool BoxInBox(const D3DXVECTOR3& _vBoxPos1, const D3DXVECTOR3& _vBoxSize1, const D3DXVECTOR3& _vBoxPos2, const D3DXVECTOR3& _vBoxSize2)
{
	_vec3 vHalfSize1 = _vBoxSize1 * 0.5f;
	_vec3 vHalfSize2 = _vBoxSize2 * 0.5f;
	if(((_vBoxPos1.x - vHalfSize1.x) > (_vBoxPos2.x + vHalfSize2.x)) || ((_vBoxPos2.x - vHalfSize2.x) > (_vBoxPos1.x + vHalfSize1.x))) return false;
	if(((_vBoxPos1.y - vHalfSize1.y) > (_vBoxPos2.y + vHalfSize2.y)) || ((_vBoxPos2.y - vHalfSize2.y) > (_vBoxPos1.y + vHalfSize1.y))) return false;
	if(((_vBoxPos1.z - vHalfSize1.z) > (_vBoxPos2.z + vHalfSize2.z)) || ((_vBoxPos2.z - vHalfSize2.z) > (_vBoxPos1.z + vHalfSize1.z))) return false;
	return true;
}

inline bool BoxInBox(const D3DXMATRIX& _matBoxWorld1, const D3DXVECTOR3& _vBoxPos1, const D3DXVECTOR3& _vBoxSize1, 
					 const D3DXMATRIX& _matBoxWorld2, const D3DXVECTOR3& _vBoxPos2, const D3DXVECTOR3& _vBoxSize2)
{
	float c[3][3];
	float absC[3][3];
	float d[3];
	float r0, r1, r;
	int i;
	bool existsParallelPair = false;
	D3DXVECTOR3 diff = (_vBoxPos1 + *(_vec3*)_matBoxWorld1.m[3]) - (_vBoxPos2 + *(_vec3*)_matBoxWorld2.m[3]);
	_vec3 vAxisDir[2][3];
	float fAxisLen[2][3];

	vAxisDir[0][0] = Normalize(*(_vec3*)_matBoxWorld1.m[0]);
	vAxisDir[0][1] = Normalize(*(_vec3*)_matBoxWorld1.m[1]);
	vAxisDir[0][2] = Normalize(*(_vec3*)_matBoxWorld1.m[2]);
	fAxisLen[0][0] = Magnitude(*(_vec3*)_matBoxWorld1.m[0])*_vBoxSize1.x*0.5f;
	fAxisLen[0][1] = Magnitude(*(_vec3*)_matBoxWorld1.m[1])*_vBoxSize1.y*0.5f;
	fAxisLen[0][2] = Magnitude(*(_vec3*)_matBoxWorld1.m[2])*_vBoxSize1.z*0.5f;

	vAxisDir[1][0] = Normalize(*(_vec3*)_matBoxWorld2.m[0]);
	vAxisDir[1][1] = Normalize(*(_vec3*)_matBoxWorld2.m[1]);
	vAxisDir[1][2] = Normalize(*(_vec3*)_matBoxWorld2.m[2]);
	fAxisLen[1][0] = Magnitude(*(_vec3*)_matBoxWorld2.m[0])*_vBoxSize2.x*0.5f;
	fAxisLen[1][1] = Magnitude(*(_vec3*)_matBoxWorld2.m[1])*_vBoxSize2.y*0.5f;
	fAxisLen[1][2] = Magnitude(*(_vec3*)_matBoxWorld2.m[2])*_vBoxSize2.z*0.5f;

	for( i = 0 ; i < 3 ; ++i )
	{
		c[0][i] = Dot( vAxisDir[0][0], vAxisDir[1][i] );
		absC[0][i] = abs( c[0][i] );
		if( absC[0][i] > CUTONE )
			existsParallelPair = true;
	}
	d[0] = Dot( diff, vAxisDir[0][0] );
	r = abs( d[0] );
	r0 = fAxisLen[0][0];
	r1 = fAxisLen[1][0] * absC[0][0] + fAxisLen[1][1] * absC[0][1] + fAxisLen[1][2] * absC[0][2];

	if( r > r0 + r1 )
		return false;

	for( i = 0 ; i < 3 ; ++i )
	{
		c[1][i] = Dot( vAxisDir[0][1], vAxisDir[1][i] );
		absC[1][i] = abs( c[1][i] );
		if( absC[1][i] > CUTONE )
			existsParallelPair = true;
	}
	d[1] = Dot( diff, vAxisDir[0][1] );
	r = abs( d[1] );
	r0 = fAxisLen[0][1];
	r1 = fAxisLen[1][0] * absC[1][0] + fAxisLen[1][1] * absC[1][1] + fAxisLen[1][2] * absC[1][2];
	if( r > r0 + r1 )
		return false;

	for( i = 0 ; i < 3 ; ++i )
	{
		c[2][i] = Dot( vAxisDir[0][2], vAxisDir[1][i] );
		absC[2][i] = abs( c[2][i] );
		if( absC[2][i] > CUTONE )
			existsParallelPair = true;
	}
	d[2] = Dot( diff, vAxisDir[0][2] );

	r = abs( d[2] );
	r0 = fAxisLen[0][2];
	r1 = fAxisLen[1][0] * absC[2][0] + fAxisLen[1][1] * absC[2][1] + fAxisLen[1][2] * absC[2][2];
	if( r > r0 + r1 )
		return false;

	r = abs( Dot( diff, vAxisDir[1][0] ) );
	r0 = fAxisLen[0][0] * absC[0][0] + fAxisLen[0][1] * absC[1][0] + fAxisLen[0][2] * absC[2][0];
	r1 = fAxisLen[1][0];
	if( r > r0 + r1 )
		return false;

	r = abs( Dot( diff, vAxisDir[1][1] ) );
	r0 = fAxisLen[0][0] * absC[0][1] + fAxisLen[0][1] * absC[1][1] + fAxisLen[0][2] * absC[2][1];
	r1 = fAxisLen[1][1];
	if( r > r0 + r1 )
		return false;

	r = abs( Dot( diff, vAxisDir[1][2] ) );
	r0 = fAxisLen[0][0] * absC[0][2] + fAxisLen[0][1] * absC[1][2] + fAxisLen[0][2] * absC[2][2];
	r1 = fAxisLen[1][2];
	if( r > r0 + r1 )
		return false;

	if( existsParallelPair == true )
		return true;

	r = abs( d[2]*c[1][0] - d[1]*c[2][0] );
	r0 = fAxisLen[0][1] * absC[2][0] + fAxisLen[0][2] * absC[1][0];
	r1 = fAxisLen[1][1] * absC[0][2] + fAxisLen[1][2] * absC[0][1];
	if( r > r0 + r1 )
		return false;

	r = abs( d[2]*c[1][1] - d[1]*c[2][1] );
	r0 = fAxisLen[0][1] * absC[2][1] + fAxisLen[0][2] * absC[1][1];
	r1 = fAxisLen[1][0] * absC[0][2] + fAxisLen[1][2] * absC[0][0];
	if( r > r0 + r1 )
		return false;

	r = abs( d[2]*c[1][2] - d[1]*c[2][2] );
	r0 = fAxisLen[0][1] * absC[2][2] + fAxisLen[0][2] * absC[1][2];
	r1 = fAxisLen[1][0] * absC[0][1] + fAxisLen[1][1] * absC[0][0];
	if( r > r0 + r1 )
		return false;

	r = abs( d[0]*c[2][0] - d[2]*c[0][0] );
	r0 = fAxisLen[0][0] * absC[2][0] + fAxisLen[0][2] * absC[0][0];
	r1 = fAxisLen[1][1] * absC[1][2] + fAxisLen[1][2] * absC[1][1];
	if( r > r0 + r1 )
		return false;

	r = abs( d[0]*c[2][1] - d[2]*c[0][1] );
	r0 = fAxisLen[0][0] * absC[2][1] + fAxisLen[0][2] * absC[0][1];
	r1 = fAxisLen[1][0] * absC[1][2] + fAxisLen[1][2] * absC[1][0];
	if( r > r0 + r1 )
		return false;

	r = abs( d[0]*c[2][2] - d[2]*c[0][2] );
	r0 = fAxisLen[0][0] * absC[2][2] + fAxisLen[0][2] * absC[0][2];
	r1 = fAxisLen[1][0] * absC[1][1] + fAxisLen[1][1] * absC[1][0];
	if( r > r0 + r1 )
		return false;

	r = abs( d[1]*c[0][0] - d[0]*c[1][0] );
	r0 = fAxisLen[0][0] * absC[1][0] + fAxisLen[0][1] * absC[0][0];
	r1 = fAxisLen[1][1] * absC[2][2] + fAxisLen[1][2] * absC[2][1];
	if( r > r0 + r1 )
		return false;

	r = abs( d[1]*c[0][1] - d[0]*c[1][1] );
	r0 = fAxisLen[0][0] * absC[1][1] + fAxisLen[0][1] * absC[0][1];
	r1 = fAxisLen[1][0] * absC[2][2] + fAxisLen[1][2] * absC[2][0];
	if( r > r0 + r1 )
		return false;

	r = abs( d[1]*c[0][2] - d[0]*c[1][2] );
	r0 = fAxisLen[0][0] * absC[1][2] + fAxisLen[0][1] * absC[0][2];
	r1 = fAxisLen[1][0] * absC[2][1] + fAxisLen[1][1] * absC[2][0];
	if( r > r0 + r1 )
		return false;

	return true;
}

inline bool BoxInRay(const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir, D3DXVECTOR3* _vColPos = NULL)
{
	_vec3 vBounds[2];
	_vec3 vInvRayDir;
	int iSign[3];
	_vec3 vMin, vMax;
	float fMin, fMax;

	vBounds[0] = _vBoxPos - _vBoxSize * 0.5f;
	vBounds[1] = _vBoxPos + _vBoxSize * 0.5f;

	for(int i = 0; i < 3; ++i) {
		vInvRayDir[i] = 1.f / _vRayDir[i];
		iSign[i] = (vInvRayDir[i] < 0);

		vMin[i] = (vBounds[iSign[i]][i] - _vRayPos[i]) * vInvRayDir[i];
		vMax[i] = (vBounds[1-iSign[i]][i] - _vRayPos[i]) * vInvRayDir[i];
	}

	fMin = vMin[0];
	fMax = vMax[0];

	for(int i = 1; i < 3; ++i) {
		if ((fMin > vMax[i]) || (vMin[i] > fMax)) return false;

		if (vMin[i] > fMin) fMin = vMin[i];
		if (vMax[i] < fMax) fMax = vMax[i];
	}

	float fT = fMin; 
	if (fT < 0) {
		fT = fMax; 
		if (fT < 0) return false;
	}

	if(_vColPos) *_vColPos = _vRayPos + _vRayDir*fT;

	return true;
}

inline bool BoxInRay(const D3DXMATRIX& _matBoxWorld, const D3DXVECTOR3& _vBoxPos, const D3DXVECTOR3& _vBoxSize, const D3DXVECTOR3& _vRayPos, const D3DXVECTOR3& _vRayDir, D3DXVECTOR3* _vColPos = NULL)
{
	_vec3 vBounds[2];
	_vec3 vInvRayDir;
	int iSign[3];
	_vec3 vMin, vMax;
	float fMin, fMax;
	_matrix matInvBoxWorld;
	_vec3 vLocalRayPos;
	_vec3 vLocalRayDir;

	if(_vBoxSize.x <= CUTZERO || _vBoxSize.y <= CUTZERO || _vBoxSize.z <= CUTZERO) return false;

	D3DXMatrixInverse(&matInvBoxWorld, NULL, &_matBoxWorld);
	vLocalRayPos = TransformCoord(_vRayPos, matInvBoxWorld);
	vLocalRayDir = TransformNormal(_vRayDir, matInvBoxWorld);

	vBounds[0] = _vBoxPos - _vBoxSize * 0.5f;
	vBounds[1] = _vBoxPos + _vBoxSize * 0.5f;

	for(int i = 0; i < 3; ++i) {
		vInvRayDir[i] = 1.f / vLocalRayDir[i];
		iSign[i] = (vInvRayDir[i] < 0);

		vMin[i] = (vBounds[iSign[i]][i] - vLocalRayPos[i]) * vInvRayDir[i];
		vMax[i] = (vBounds[1-iSign[i]][i] - vLocalRayPos[i]) * vInvRayDir[i];
	}

	fMin = vMin[0];
	fMax = vMax[0];

	for(int i = 1; i < 3; ++i) {
		if ((fMin > vMax[i]) || (vMin[i] > fMax)) return false;

		if (vMin[i] > fMin) fMin = vMin[i];
		if (vMax[i] < fMax) fMax = vMax[i];
	}

	float fT = fMin; 
	if (fT < 0) {
		fT = fMax; 
		if (fT < 0) return false;
	}

	if(_vColPos) *_vColPos = TransformCoord(vLocalRayPos + vLocalRayDir*fT, _matBoxWorld);

	return true;
}

//////////////////////////////////////////////////////////////////////////