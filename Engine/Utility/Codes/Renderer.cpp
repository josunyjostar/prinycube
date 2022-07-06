#include "Renderer.h"
#include "RenderManager.h"
using namespace Engine;

void CRenderer::SetRenderType(RENDERING_TYPE _eRenderType)
{
	CRenderManager::Inst().DeleteRenderer(this);
	m_eRenderType = _eRenderType;
	CRenderManager::Inst().AddRenderer(this);
}

void CRenderer::SetZOrder(_ushort _wZOrder)
{
	if(m_eRenderType == RENDER_PRIORITY || m_eRenderType == RENDER_UI)
	{
		CRenderManager::Inst().DeleteRenderer(this);
		m_wZOrder = _wZOrder;
		CRenderManager::Inst().AddRenderer(this);
	}
	else
		m_wZOrder = _wZOrder;
}

void CRenderer::Enable()
{
	CRenderManager::Inst().DeleteRenderer(this);
	CRenderManager::Inst().AddRenderer(this);
}

void CRenderer::Disable()
{
	CRenderManager::Inst().DeleteRenderer(this);
}