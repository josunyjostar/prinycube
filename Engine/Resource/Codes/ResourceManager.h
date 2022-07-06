#ifndef ResourceManager_h__
#define ResourceManager_h__

#include "TemplateSingleton.h"
#include "TinyHashTable.h"

#include "Texture.h"

#include "Triangle_Color.h"
#include "Rect_Color.h"
#include "Rect_Texture.h"
#include "Cube_Color.h"
#include "Cube_Texture.h"
#include "Cube_CubeTexture.h"
#include "Plane_Color.h"
#include "Plane_Texture.h"
#include "Terrain.h"

BEGIN(Engine)

class ENGINE_DLL CResourceManager : public TSingleton<CResourceManager>
{
private:
	explicit CResourceManager();
	virtual ~CResourceManager();

public:
	friend TSingleton;

	HRESULT			Init(RCP_DEVICE& _pDev, const _ushort& _wSize);
	void			ContainerClear(const _ushort& _wContainerIdx) { m_hResources[_wContainerIdx].Clear(); }

	template<typename T>
	RCPtr<T>		FindResource(const _ushort& _wContainerIdx, const _char* _pResourceTag) {
		return m_hResources[_wContainerIdx][_pResourceTag]; }
	
	template<typename T>
	RCPtr<T>		CreateBuffer(const _ushort& _wContainerIdx, const _char* _pResourceTag)
	{
		RCP_RESOURCE& pResource = m_hResources[_wContainerIdx][_pResourceTag];
		if(!pResource) {
			pResource = CComponent::Create<T>();
			pResource->SetTag(_pResourceTag);
			if(FAILED(pResource->InitResource(m_pDev))) pResource = NULL;
		}
		return pResource;
	}

	RCP_TEXTURE		CreateTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag,
		TEXTURE_TYPE _eType, const _char* _pFilePath, _ushort _iCnt = 1, _ushort _iStartNumber = 0);

private:
	typedef TinyHashTable<const _char*, RCP_RESOURCE>	HASH_RESOURCE;
	HASH_RESOURCE*	m_hResources;
	_ushort			m_ResourceSize;

	RCP_DEVICE		m_pDev;

public:
	void			Release();
};

END

#endif // ResourceManager_h__
