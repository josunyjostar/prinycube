#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine {

	class CGameObject;
	typedef struct tagCollision_Info {
		tagCollision_Info():pObj(NULL) {}
		tagCollision_Info(CGameObject* _pObj, _vec3 _vColPos = _vec3())
			:pObj(_pObj), vColPos(_vColPos) {}

		CGameObject* pObj;
		_vec3 vColPos;
	}COL_INFO;

	typedef struct tagVertex_Color
	{
		tagVertex_Color() {}
		tagVertex_Color(const _vec3& _vec, const _ulong& _color)
			:vPosition(_vec), dwColor(_color) {}

		_vec3	vPosition;
		_ulong	dwColor;
	}VTXCOL;
#define D3DFVF_VTXCOL D3DFVF_XYZ|D3DFVF_DIFFUSE

	typedef struct tagVertex_Texture
	{
		tagVertex_Texture() {}
		tagVertex_Texture(const _vec3& _vec, const _vec2& _uv)
			:vPosition(_vec), vUV(_uv) {}

		_vec3	vPosition;
		_vec2	vUV;
	}VTXTEX;
#define D3DFVF_VTXTEX D3DFVF_XYZ|D3DFVF_TEX1

	typedef struct tagCube_Texture
	{
		tagCube_Texture() {}
		tagCube_Texture(const _vec3& _vec, const _vec3& _uv)
			:vPosition(_vec), vUV(_uv) {}

		_vec3	vPosition;
		_vec3	vUV;
	}VTXCUBETEX;
#define D3DFVF_VTXCUBETEX D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0)

	typedef struct tagVertex_Normal_Texture
	{
		tagVertex_Normal_Texture() {}
		tagVertex_Normal_Texture(const _vec3& _vec, const _vec3& _uv)
			:vPosition(_vec), vUV(_uv) {}

		_vec3	vPosition;
		_vec3	vNormal;
		_vec2	vUV;
	}VTXNORMALTEX;
#define D3DFVF_VTXNORMALTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1

}

#endif // Engine_Struct_h__