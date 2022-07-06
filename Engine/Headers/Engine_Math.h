#ifndef Engine_Math_h__
#define Engine_Math_h__

// tagVector2 [ _vec2 ]
//////////////////////////////////////////////////////////////////////////

typedef struct tagVector2 : public D3DXVECTOR2 {
	tagVector2()										:D3DXVECTOR2(0.f, 0.f)			{}
	tagVector2( const float* _pFloat )					:D3DXVECTOR2(_pFloat)			{}
	tagVector2( const float& _f )						:D3DXVECTOR2(_f, _f)			{}
	tagVector2( const D3DXVECTOR2& _rVec )				:D3DXVECTOR2(_rVec)				{}
	tagVector2( const D3DXFLOAT16* _pFloat16 )			:D3DXVECTOR2(_pFloat16)			{}
	tagVector2( float _x, float _y )					:D3DXVECTOR2(_x, _y)			{}
}_vec2;


// tagVector3 [ _vec3 ]
//////////////////////////////////////////////////////////////////////////

typedef struct tagVector3 : public D3DXVECTOR3 {
	tagVector3()										:D3DXVECTOR3(0.f, 0.f, 0.f)		{}
	tagVector3( const float* _pFloat )					:D3DXVECTOR3(_pFloat)			{}
	tagVector3( const float& _f )						:D3DXVECTOR3(_f, _f, _f)		{}
	tagVector3( const D3DVECTOR& _rVec )				:D3DXVECTOR3(_rVec)				{}
	tagVector3( const D3DXFLOAT16* _pFloat16 )			:D3DXVECTOR3(_pFloat16)			{}
	tagVector3( float _x, float _y, float _z )			:D3DXVECTOR3(_x, _y, _z)		{}
	tagVector3( const D3DXVECTOR2* _pVec2 )				:D3DXVECTOR3(_pVec2->x, _pVec2->y, 0.f) {}
	
	inline friend D3DXVECTOR3 operator*(const D3DXVECTOR3& _vDest, const D3DXVECTOR3& _vSour);
}_vec3;


// tagVector4 [ _vec4 ]
//////////////////////////////////////////////////////////////////////////

typedef struct tagVector4 : public D3DXVECTOR4 {
	tagVector4()										:D3DXVECTOR4(0.f, 0.f, 0.f, 0.f){}
	tagVector4( const float* _pFloat )					:D3DXVECTOR4(_pFloat)			{}
	tagVector4( const float& _f )						:D3DXVECTOR4(_f, _f, _f, _f)	{}
	tagVector4( const D3DXFLOAT16* _pFloat16 )			:D3DXVECTOR4(_pFloat16)			{}
	tagVector4( const D3DVECTOR& _rVec, FLOAT _w )		:D3DXVECTOR4(_rVec, _w)			{}
	tagVector4( float _x, float _y, float _z, float _w ):D3DXVECTOR4(_x, _y, _z, _w)	{}
	tagVector4( float _rgb, float _w )					:D3DXVECTOR4(_rgb, _rgb, _rgb, _w)	{}

	inline friend D3DXVECTOR4 operator*(const D3DXVECTOR4& _vDest, const D3DXVECTOR4& _vSour);
}_vec4;


// tagMatrix [ _matrix ]
//////////////////////////////////////////////////////////////////////////

typedef struct tagMatrix : public D3DXMATRIX {
	tagMatrix() :D3DXMATRIX( 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f ) {}
	tagMatrix( const float* _pFloat )					:D3DXMATRIX(_pFloat)			{}
	tagMatrix( const D3DMATRIX& _rMat )					:D3DXMATRIX(_rMat)				{}
	tagMatrix( const D3DXFLOAT16* _pFloat16 )			:D3DXMATRIX(_pFloat16)			{}
	tagMatrix( float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44 )
		:D3DXMATRIX( _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 ) {}
}_matrix;


// tagRay [ _ray ]
//////////////////////////////////////////////////////////////////////////

typedef struct tagRay {
	tagRay() {}
	tagRay( const D3DXVECTOR3* _pPos, const D3DXVECTOR3* _pDir )	:Pos(*_pPos), Dir(*_pDir)								{}
	tagRay( const D3DMATRIX* _pMat )								:Pos(*(_vec3*)_pMat->m[3]), Dir(*(_vec3*)_pMat->m[2])	{}

	_vec3 Pos;
	_vec3 Dir;
}_ray;

//////////////////////////////////////////////////////////////////////////

#include "Engine_Math_Macro.inl"
#include "Engine_Math_Function.inl"
#include "Engine_Math_Method.inl"


#endif // Engine_Math_h__
