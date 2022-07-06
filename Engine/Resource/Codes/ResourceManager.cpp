#include "ResourceManager.h"
using namespace Engine;

CResourceManager::CResourceManager()
:m_hResources(NULL),
m_ResourceSize(0)
{
}

CResourceManager::~CResourceManager()
{
	this->Release();
}

HRESULT CResourceManager::Init(RCP_DEVICE& _pDev, const _ushort& _wSize)
{
	if(m_hResources) return E_FAIL;

	m_hResources = new HASH_RESOURCE[_wSize];
	m_ResourceSize = _wSize;

	m_pDev = _pDev;

	return S_OK;
}

RCP_TEXTURE CResourceManager::CreateTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag,
											  TEXTURE_TYPE _eType, const _char* _pFilePath, _ushort _iCnt /*= 1*/, _ushort _iStartNumber /*= 0*/)
{
	RCP_RESOURCE& pResource = m_hResources[_wContainerIdx][_pResourceTag];
	if(!pResource) {
		pResource = CComponent::Create<CTexture>();
		pResource->InitResource(m_pDev);
		pResource->SetTag(_pResourceTag);
		if(FAILED(((RCP_TEXTURE)pResource)->CreateTexture(_eType, _pFilePath, _iCnt, _iStartNumber))) pResource = NULL;
	}
	return pResource;
}

void CResourceManager::Release()
{
	Safe_Delete_Array(m_hResources);
	m_ResourceSize = 0;

	m_pDev = NULL;
}