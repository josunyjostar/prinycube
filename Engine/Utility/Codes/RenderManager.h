#ifndef RenderManager_h__
#define RenderManager_h__

#include "Renderer.h"
#include "Transform.h"
#include "TemplateSingleton.h"

#include <list>
using namespace std;

BEGIN(Engine)

class ENGINE_DLL CRenderManager : public TSingleton<CRenderManager>
{
private:
	explicit CRenderManager();
	virtual ~CRenderManager();

public:
	friend TSingleton;

	HRESULT	Init(RCP_DEVICE& _pDev, const _ushort& _wSize);
	void	Render();

	void	AddRenderer(CRenderer* _pRenderer);
	void	DeleteRenderer(CRenderer* _pRenderer);

private:
	typedef struct tagZSortRendererNode {
		tagZSortRendererNode(CRenderer* _pRenderer):fZ(0.f), pRenderer(_pRenderer) {
			pTransform = &*pRenderer->GetGameObject()->GetComponent<CTransform>(); }
		CRenderer*	operator->() const					{ return pRenderer; }
		bool		operator==(CRenderer* _pRenderer)	{ return pRenderer==_pRenderer; }

		float		fZ;
		CRenderer*	pRenderer;
		CTransform*	pTransform;

		void CalcZ(_matrix _matView) { fZ = (pTransform)?TransformCoord(pTransform->GetPosition(), _matView).z:0.f; }
	}ZSORT_NODE;

	struct ZSortLess {
		bool operator() (const ZSORT_NODE& a, const ZSORT_NODE& b) 
		{ return a.fZ > b.fZ; }	
	};

private:
	typedef list<CRenderer*>	LIST_RENDER;
	
	LIST_RENDER*		m_PriorityList;
	LIST_RENDER			m_BasicList;
	LIST_RENDER			m_AlphaTestList;
	list<ZSORT_NODE>	m_AlphaBlendList;
	LIST_RENDER*		m_UIList;
	_ushort				m_wMaxZSize;

	RCP_DEVICE			m_pDev;

private:
	void AlphaSorting();

	template<typename T>
	void ListRender(T& _List) {
		T::iterator Iter = _List.begin();
		T::iterator EndIter = _List.end();
		while(Iter != EndIter) {
			(*Iter)->Render();
			Iter++;
		}
	}

	template<typename T, typename T2>
	T2 ListFind(T& _List, CRenderer* _pRenderer) {
		T2 Iter = _List.begin();
		T2 EndIter = _List.end();
		while(Iter != EndIter) {
			if(*Iter == _pRenderer) return Iter;
			Iter++;
		}
		return EndIter;
	}

	template<typename T>
	void ListDelete(T& _List, CRenderer* _pRenderer) {
		T::iterator Iter = _List.begin();
		T::iterator EndIter = _List.end();
		while(Iter != EndIter) {
			if(*Iter == _pRenderer) {
				_List.erase(Iter);
				return;
			}
			Iter++;
		}
	}

public:
	void Release();
};

END

#endif // RenderManager_h__