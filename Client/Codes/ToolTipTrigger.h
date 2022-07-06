#ifndef ToolTipTrigger_h__
#define ToolTipTrigger_h__

#include "Include.h"

class CToolTipTrigger : public Engine::CGameObject
{
protected:
	explicit CToolTipTrigger();
	virtual ~CToolTipTrigger();

public:
	DeclRTTI(CToolTipTrigger, Engine::CGameObject);

	void			SetPosition(const _vec3& _vPos) { m_pTransform->SetPosition(_vPos); }
	void			SetToolTip(const _ushort& _pResIdx, const char* _pResName, float _fLifeTime, float _fUpTime, float _fDownTime, const _vec2 _vScale, const _vec2 _vOffset);

public:
	virtual void	TriggerEnter(Engine::COL_INFO& _ColInfo);

private:
	RCP_DEVICE		m_pGraphicDev;
	RCP_TRANSFORM	m_pTransform;
	_ushort			m_wResourceIdx;
	const char*		m_pResourceName;
	float			m_fLifeTime;
	float			m_fUpTime;
	float			m_fDownTime;
	_vec2			m_vScale;
	_vec2			m_vOffSet;

public:
	CREATE(CToolTipTrigger);
};

TYPEDEF_RCP(CToolTipTrigger, TOOLTIPTRIGGER);

#endif // ToolTipTrigger_h__
