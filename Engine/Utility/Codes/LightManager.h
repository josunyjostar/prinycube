#ifndef LightManager_h__
#define LightManager_h__

#include "TemplateSingleton.h"
#include "TinyHashTable.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightManager : public TSingleton<CLightManager>
{
private:
	explicit CLightManager();
	virtual ~CLightManager();

public:
	friend TSingleton;

	RCP_LIGHT	GetLight(const _char* _pLightName)	{ return m_hLight[_pLightName]; }

public:
	void		Init(RCP_DEVICE& _pDev)				{ m_pDev = _pDev; }

	template<typename T>
	RCPtr<T>	CreateLight(const _char* _pLightName, const D3DLIGHT9* _Light = NULL) {
		RCP_LIGHT& pLight = m_hLight[_pLightName];
		if(!pLight) {
			pLight = T::Create(false);
			pLight->SetTag(_pLightName);
			pLight->InitLight(m_pDev, _Light);
		}
		return pLight;
	}

private:
	typedef TinyHashTable<const _char*, RCP_LIGHT> HASH_LIGHT;
	HASH_LIGHT	m_hLight;

	RCP_DEVICE	m_pDev;

public:
	void Release();
};

END

#endif // LightManager_h__
