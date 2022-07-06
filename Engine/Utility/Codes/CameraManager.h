#ifndef CameraManager_h__
#define CameraManager_h__

#include "TemplateSingleton.h"
#include "TinyHashTable.h"
#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CCameraManager : public TSingleton<CCameraManager>
{
private:
	explicit CCameraManager();
	virtual ~CCameraManager();

public:
	friend TSingleton;

	RCP_CAMERA	SetMainCam(const _char* _pCameraName)	{ return m_pMainCam = m_hCamera[_pCameraName]; }
	RCP_CAMERA	GetCamera(const _char* _pCameraName)	{ return m_hCamera[_pCameraName]; }
	RCP_CAMERA	GetMainCam() 							{ return m_pMainCam; }

public:
	void		Init(RCP_DEVICE& _pDev)					{ m_pDev = _pDev; }
	void		Update(const _float& _fDeltaTime);

	template<typename T>
	RCPtr<T>	CreateCamera(const _char* _pCameraName, bool _bMainCam = false){
		RCP_CAMERA& pCamera = m_hCamera[_pCameraName];
		if(!pCamera) {
			pCamera = T::Create(false);
			pCamera->SetTag(_pCameraName);
			pCamera->SetGraphicDev(m_pDev);
			if(_bMainCam) m_pMainCam = pCamera;
		}
		return pCamera;
	}

private:
	typedef TinyHashTable<const _char*, RCP_CAMERA> HASH_CAMERA;
	HASH_CAMERA	m_hCamera;
	RCP_CAMERA	m_pMainCam;

	RCP_DEVICE	m_pDev;

public:
	void Release();
};

END

#endif // CameraManager_h__
