#ifndef Collider_h__
#define Collider_h__

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "TinyHashTable.h"

#include <list>
using namespace std;

enum COL_TYPE { COL_SPHERE, COL_CUBE, COL_RAY, COL_END };

BEGIN(Engine)

enum COL_STATE { CS_NULL, CS_ENTER, CS_COL, CS_EXIT };

class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider():m_ColType(COL_END), m_bTrigger(false) {}
	virtual ~CCollider() {}

public:
	DeclRTTI(CCollider, CComponent);
	friend CComponent;

	template<typename T>
	RCPtr<T>		GetInst() const				{ return (T*)this; }
	int				GetColType() const			{ return m_ColType; }
	RCP_TRANSFORM	GetTransform() const		{ return m_pTransform; }
	COL_STATE		GetColState(CCollider* _pCol, bool _bCol);

	void			SetTrigger(bool _bTrigger)	{ m_bTrigger = _bTrigger; }
	bool			IsTrigger() const			{ return m_bTrigger; }

public:
	virtual void	Enable();
	virtual void	Disable();
	virtual HRESULT Init();

private:
	void			Remove_PrevCol(CCollider* _pCol);

protected:
	typedef list<CCollider*> LIST_PREVCOL;
	LIST_PREVCOL	m_PrevColList;
	RCP_TRANSFORM	m_pTransform;
	COL_TYPE		m_ColType;
	bool			m_bTrigger;
};

END

TYPEDEF_RCP(Engine::CCollider, COLLIDER);

#endif // Collider_h__
