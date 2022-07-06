#ifndef SkyBox_h__
#define SkyBox_h__

#include "Include.h"

class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox();
	virtual ~CSkyBox();

public:
	DeclRTTI(CSkyBox, Engine::CGameObject);

	virtual HRESULT Init();
	virtual void Render();

public:
	RCP_DEVICE			m_pGraphicDev;
	RCP_CUBE_CUBETEX	m_pResource;
	RCP_TEXTURE			m_pTexture;
	RCP_TRANSFORM		m_pTransform;

public:
	CREATE(CSkyBox);
};

TYPEDEF_RCP(CSkyBox, SKYBOX);

#endif // SkyBox_h__
