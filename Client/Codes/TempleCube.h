#ifndef TempleCube_h__
#define TempleCube_h__

#include "BasicCube.h"
#include "TempleLightSource.h"

class CTempleCube : public CBasicCube
{
protected:
	explicit CTempleCube();
	virtual ~CTempleCube();

public:
	DeclRTTI(CTempleCube, CBasicCube);

	void CreateLight(RCP_LAYER _pLayer);

public:
	virtual HRESULT Init();

public:
	CREATE(CTempleCube);
};

TYPEDEF_RCP(CTempleCube, TEMPLECUBE);

#endif // TempleCube_h__
