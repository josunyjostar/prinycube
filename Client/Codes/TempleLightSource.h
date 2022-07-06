#ifndef TempleLightSource_h__
#define TempleLightSource_h__

#include "BasicCube.h"
#include "TempleLight.h"

class CTempleLightSource : public CBasicCube
{
protected:
	explicit CTempleLightSource();
	virtual ~CTempleLightSource();

public:
	DeclRTTI(CTempleLightSource, CBasicCube);

	void SetHide(bool _bHide, bool _bInit = false);

public:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Render();

public:
	CREATE(CTempleLightSource);
};

TYPEDEF_RCP(CTempleLightSource, TEMPLELIGHTSORUCE);

#endif // TempleLightSource_h__
