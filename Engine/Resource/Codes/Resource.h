#ifndef Resource_h__
#define Resource_h__

#include "Engine_Include.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResource abstract : public CComponent
{
protected:
	explicit CResource() {}
	virtual ~CResource() {}

public:
	DeclRTTI(CResource, CComponent);

	virtual HRESULT InitResource(RCP_DEVICE& _pGraphicDev) { m_pGraphicDev = _pGraphicDev; return S_OK; }
	
	template<typename T>
	RCPtr<T> LoadResource(const _ushort& _wContainerIdx, const _char* _pResourceTag) {
		return &((T*)this)->operator=(*CResourceManager::Inst().FindResource<T>(_wContainerIdx, _pResourceTag));
	}

protected:
	RCP_DEVICE m_pGraphicDev;
};

END

TYPEDEF_RCP(Engine::CResource, RESOURCE);

#endif // Resource_h__
