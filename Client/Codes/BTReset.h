#ifndef BTReset_h__
#define BTReset_h__

#include "Button.h"
#include "StageScene.h"

class CBTReset : public CButton
{
protected:
	explicit CBTReset() {}
	virtual ~CBTReset() {}

public:
	DeclRTTI(CBTReset, CButton);

public:
	virtual void	OnMouseButtonUp()	{ ((RCP_STAGESCENE)Engine::CSceneManager::Inst().GetRunningScene())->Reset(); }

public:
	CREATE(CBTReset);
};

TYPEDEF_RCP(CBTReset, BTRESET);

#endif // BTReset_h__
