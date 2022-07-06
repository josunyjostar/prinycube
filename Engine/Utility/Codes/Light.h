#ifndef Light_h__
#define Light_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CGameObject
{
protected:
	explicit CLight();
	virtual ~CLight();

public:
	DeclRTTI(CLight, CGameObject);

	void SetLight(const D3DLIGHT9& _Light)	{ m_Light = _Light; }
	D3DLIGHT9 GetLight() const				{ return m_Light; }

public:
	void InitLight(RCP_DEVICE& _pGraphicDev, const D3DLIGHT9* _Light);
	void DeviceInLight();

protected:
	RCP_DEVICE		m_pGraphicDev;
	D3DLIGHT9		m_Light;

public:
	CREATE(CLight);
};

END

TYPEDEF_RCP(Engine::CLight, LIGHT);

#endif // Light_h__
