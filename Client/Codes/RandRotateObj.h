#ifndef RandRotateObj_h__
#define RandRotateObj_h__

#include "Effect.h"

class CRandRotateObj : public CEffect
{
protected:
	explicit CRandRotateObj() {}
	virtual ~CRandRotateObj() {}

public:
	DeclRTTI(CRandRotateObj, CEffect);

	virtual HRESULT Init();

public:
	CREATE(CRandRotateObj);
};

TYPEDEF_RCP(CRandRotateObj, RANDROTATEOBJ);

#endif // RandRotateObj_h__
