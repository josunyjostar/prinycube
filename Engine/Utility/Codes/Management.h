#ifndef Scene_h__
#define Scene_h__

#include "GameObject.h"

#include <list>
using namespace std;

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene();
	virtual ~CScene();

public:
	DeclRTTI(CScene, CBase);

	virtual HRESULT Ready_Scene() {return S_OK;}
	virtual HRESULT Start_Scene() {return S_OK;}
	virtual void Update_Scene();
	virtual void Render_Scene();
	virtual void Clear_Scene();

protected:
	list<RCP_GAMEOBJECT>	m_GameObjectList;

public:
	static CScene* Create() {return new CScene;}
};

END

TYPEDEF_RCP(Engine::CScene, SCENE);

#endif // Scene_h__
