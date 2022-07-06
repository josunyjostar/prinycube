#ifndef Renderer_h__
#define Renderer_h__

#include "GameObject.h"

BEGIN(Engine)

enum RENDERING_TYPE { RENDER_NORMAL, RENDER_ALPHA_TEST, RENDER_ALPHA_BLEND, RENDER_PRIORITY, RENDER_UI, RENDER_END };

class ENGINE_DLL CRenderer : public CComponent
{
protected:
	explicit CRenderer():m_eRenderType(RENDER_NORMAL), m_wZOrder(0) {}
	virtual ~CRenderer() {}

public:
	DeclRTTI(CRenderer, CComponent);
	friend CComponent;

public:
	void			SetRenderType(RENDERING_TYPE _eRenderType);
	void			SetZOrder(_ushort _wZOrder);

	RENDERING_TYPE	GetRenderType() const	{ return m_eRenderType; }
	_ushort			GetZOrder() const		{ return m_wZOrder; }

public:
	virtual void	Enable();
	virtual void	Disable();
	virtual void	Render()				{ m_GameObject->Render(); }

protected:
	RENDERING_TYPE	m_eRenderType;
	_ushort			m_wZOrder;
};

END

TYPEDEF_RCP(Engine::CRenderer, RENDERER);

#endif // Renderer_h__
