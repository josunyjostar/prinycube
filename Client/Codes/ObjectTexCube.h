#ifndef ObjectTexCube_h__
#define ObjectTexCube_h__

#include "Include.h"

class CObjectTexCube : public Engine::CGameObject
{
private:
	explicit CObjectTexCube();
	virtual ~CObjectTexCube();

public:
	DeclRTTI(CObjectTexCube, Engine::CGameObject);

	void SetMatrial(const D3DMATERIAL9& _matrial) { m_pCube->SetMatrial(_matrial); }
	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag);

public:
	virtual HRESULT Init();
	virtual void Render();

public:
	RCP_DEVICE		m_pGraphicDev;
	RCP_CUBE_TEX	m_pCube;
	RCP_TEXTURE		m_pTexture;
	RCP_TRANSFORM	m_pTransform;

public:
	CREATE(CObjectTexCube);
};

TYPEDEF_RCP(CObjectTexCube, OBJECTTEXCUBE);

#endif // ObjectTexCube_h__
