#ifndef RayDrawing_h__
#define RayDrawing_h__

#include "Include.h"
#include "ShadowLight.h"


class CRayDrawing : public Engine::CGameObject
{
protected:
	explicit CRayDrawing();
	virtual ~CRayDrawing();

public:
	DeclRTTI(CRayDrawing, Engine::CGameObject);

	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, int _TexIndex = 0);
	void SetPos(const _vec3& _vPos) {m_pTransform->SetPosition(_vPos);}
	void SetDir(const _vec3& _vDir);
	void SetRay(const _ray& _Ray);
	void SetRayEnd(const _vec3& _vPos);
	void SetColor(const _vec3& _vColor) { m_vColor.x = _vColor.x+0.2f; m_vColor.y = _vColor.y+0.2f; m_vColor.z = _vColor.z+0.2f;}

public:
	virtual HRESULT		Init();
	virtual void		Render();

protected:
	RCP_DEVICE			m_pGraphicDev;
	RCP_TRANSFORM		m_pTransform;
	RCP_RC_TEX			m_pResource;
	RCP_TEXTURE			m_pTexture;
	RCP_SHADER			m_pColorShader;
	_vec4				m_vColor;


public:
	CREATE(CRayDrawing);
};

TYPEDEF_RCP(CRayDrawing, RAYDRAW);

#endif // RayDrawing_h__
