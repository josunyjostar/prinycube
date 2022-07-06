#ifndef Ray_h__
#define Ray_h__

#include "Include.h"
#include "RayDraw.h"

class CRay : public Engine::CGameObject
{
protected:
	explicit CRay();
	virtual ~CRay();

public:
	DeclRTTI(CRay, Engine::CGameObject);

	void SetStartRay(_ray _Ray) {m_StartRay = _Ray; m_vEndPos = _Ray.Pos + _Ray.Dir * 300;}
	void SetStartRay(_vec3 _Pos, _vec3 _Dir) {m_StartRay = _ray(&_Pos, &_Dir); m_vEndPos = _Pos + _Dir * 300;}
	void SetDrawRay(RCP_RAYDRAW _pDraw) {m_pDraw = _pDraw;}
	void SetColor(const _vec3& _vColor) { m_vColor = _vColor;}
	_ray GetStartRay(){return m_StartRay;};

public:
	virtual HRESULT		Init();
	virtual void		Update();
	virtual void		Render();

protected:
	RCP_DEVICE			m_pGraphicDev;
	RCP_RENDERER		m_pRenderer;
	RCP_RAYDRAW			m_pDraw;

	_ray				m_StartRay;
	_vec3				m_vEndPos;
	_vec3				m_vColor;
	list<_ray>			m_RayList;

	Engine::CCollisionManager* m_pCollisionMgr;
	

public:
	CREATE(CRay);
};

TYPEDEF_RCP(CRay, RAYOBJ);

#endif // Ray_h__
