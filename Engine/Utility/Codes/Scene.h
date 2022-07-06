#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

#include <map>
#include <list>
using namespace std;

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene();
	virtual ~CScene();

public:
	DeclRTTI(CScene, CBase);

	RCP_LAYER GetLayer(const _char* _pLayerName);

public:
	virtual void	Enable();
	virtual void	Disable();
	virtual void	Update();
	virtual void	Clear();

	RCP_LAYER		Create_Layer(const _char* _pLayerName);
	void			Remove_Layer(const _char* _pLayerName);
	RCP_LAYER		Find_Layer(const _char* _pLayerName);
	void			Remove_Object(const _char* _pLayerName, const RCP_GAMEOBJECT _pObj);
	int				GetObjCount()
	{
		MAP_LAYER::iterator Iter = m_mapLayer.begin();
		MAP_LAYER::iterator EndIter = m_mapLayer.end();
		int c = 0;
		while(Iter != EndIter)
		{
			c += Iter->second->GetObjCount();
			Iter++;
		}
		return c;
	}

private:
	typedef map<const _char*, RCP_LAYER>	MAP_LAYER;
	MAP_LAYER		m_mapLayer;
	list<RCP_LAYER>	m_RemoveLayerList;

public:
	CREATE(CScene);
};

END

TYPEDEF_RCP(Engine::CScene, SCENE);

#endif // Scene_h__
