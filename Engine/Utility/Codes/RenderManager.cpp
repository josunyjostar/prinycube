#include "RenderManager.h"
using namespace Engine;

CRenderManager::CRenderManager()
:m_PriorityList(NULL)
,m_UIList(NULL)
,m_wMaxZSize(0)
{
}

CRenderManager::~CRenderManager()
{
	this->Release();
}

HRESULT CRenderManager::Init(RCP_DEVICE& _pDev, const _ushort& _wSize)
{
	if(m_UIList) return E_FAIL;

	m_PriorityList = new LIST_RENDER[_wSize];
	m_UIList = new LIST_RENDER[_wSize];
	m_wMaxZSize = _wSize;
	m_pDev = _pDev;

	m_pDev->SetRenderState(D3DRS_ALPHAREF, 0x08);
	m_pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

void CRenderManager::Render()
{
	m_pDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDev->BeginScene();


	m_pDev->SetRenderState(D3DRS_ZENABLE, false);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	for(_ushort i = 0 ; i < m_wMaxZSize; ++i)
		this->ListRender(m_PriorityList[i]);

	m_pDev->SetRenderState(D3DRS_ZENABLE, true);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, true);


	this->ListRender(m_BasicList);


	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);

	this->ListRender(m_AlphaTestList);

	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	this->AlphaSorting();
	this->ListRender(m_AlphaBlendList);

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	m_pDev->SetRenderState(D3DRS_ZENABLE, false);

	for(_ushort i = 0 ; i < m_wMaxZSize; ++i)
		this->ListRender(m_UIList[i]);

	m_pDev->EndScene();
	m_pDev->Present(NULL, NULL, NULL, NULL);
}

void CRenderManager::AddRenderer(CRenderer* _pRenderer)
{
	switch(_pRenderer->GetRenderType())
	{
	case RENDER_NORMAL:
		m_BasicList.push_back(_pRenderer);
		break;
	case RENDER_ALPHA_TEST:
		m_AlphaTestList.push_back(_pRenderer);
		break;
	case RENDER_ALPHA_BLEND:
		m_AlphaBlendList.push_back(ZSORT_NODE(_pRenderer));
		break;
	case RENDER_PRIORITY: {
		_ushort wZOrder = _pRenderer->GetZOrder();
		if(wZOrder < m_wMaxZSize) m_PriorityList[wZOrder].push_back(_pRenderer);
		break;
	}
	case RENDER_UI: {
		_ushort wZOrder = _pRenderer->GetZOrder();
		if(wZOrder < m_wMaxZSize) m_UIList[wZOrder].push_back(_pRenderer);
		break;
	}
	}
}

void CRenderManager::DeleteRenderer(CRenderer* _pRenderer)
{
	switch(_pRenderer->GetRenderType())
	{
	case RENDER_NORMAL:			this->ListDelete(m_BasicList, _pRenderer);		break;
	case RENDER_ALPHA_TEST:		this->ListDelete(m_AlphaTestList, _pRenderer);	break;
	case RENDER_ALPHA_BLEND:	this->ListDelete(m_AlphaBlendList, _pRenderer);	break;
	case RENDER_PRIORITY: {
		_ushort wZOrder = _pRenderer->GetZOrder();
		if(wZOrder < m_wMaxZSize) this->ListDelete(m_PriorityList[wZOrder], _pRenderer);
		break;
	}
	case RENDER_UI: {
		_ushort wZOrder = _pRenderer->GetZOrder();
		if(wZOrder < m_wMaxZSize) this->ListDelete(m_UIList[wZOrder], _pRenderer);
		break;
	}
	}
}

void CRenderManager::AlphaSorting()
{
	list<ZSORT_NODE>::iterator Iter = m_AlphaBlendList.begin();
	list<ZSORT_NODE>::iterator EndIter = m_AlphaBlendList.end();

	_matrix matView;
	m_pDev->GetTransform(D3DTS_VIEW, &matView);

	while(Iter != EndIter) Iter++->CalcZ(matView);

	m_AlphaBlendList.sort(ZSortLess());
}

void CRenderManager::Release()
{
	Safe_Delete_Array(m_PriorityList);
	m_BasicList.clear();
	m_AlphaTestList.clear();
	m_AlphaBlendList.clear();
	Safe_Delete_Array(m_UIList);
	m_wMaxZSize = 0;
	m_pDev = NULL;
}