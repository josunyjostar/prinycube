#include "Scene.h"
using namespace Engine;

CScene::CScene()
{
}

CScene::~CScene()
{
	this->Clear();
	m_RemoveLayerList.clear();
	m_mapLayer.clear();
}

RCP_LAYER CScene::GetLayer(const _char* _pLayerName)
{
	MAP_LAYER::iterator Iter = m_mapLayer.find(_pLayerName);

	if(Iter == m_mapLayer.end())
		return NULL;
	else
		return Iter->second;
}

void CScene::Enable()
{
	MAP_LAYER::iterator Iter = m_mapLayer.begin();
	MAP_LAYER::iterator EndIter = m_mapLayer.end();

	while(Iter != EndIter) {
		if(Iter->second) 
		{
			Iter->second->SetActive(true);
			Iter++;
		}
		else Iter = m_mapLayer.erase(Iter);
	}
}

void CScene::Disable()
{
	MAP_LAYER::iterator Iter = m_mapLayer.begin();
	MAP_LAYER::iterator EndIter = m_mapLayer.end();

	while(Iter != EndIter) {
		if(Iter->second) 
		{
			Iter->second->SetActive(false);
			Iter++;
		}
		else Iter = m_mapLayer.erase(Iter);
	}
}

void CScene::Update()
{
	MAP_LAYER::iterator Iter = m_mapLayer.begin();
	MAP_LAYER::iterator EndIter = m_mapLayer.end();

	while(Iter != EndIter) {
		if(Iter->second) 
		{
			Iter->second->Update();
			Iter++;
		}
		else Iter = m_mapLayer.erase(Iter);
	}

	m_RemoveLayerList.clear();
}

void CScene::Clear()
{
	CScene::Disable();
	m_mapLayer.clear();
}

RCP_LAYER CScene::Create_Layer(const _char* _pLayerName)
{
	RCP_LAYER& pLayer = m_mapLayer[_pLayerName];
	if(!pLayer) 
	{
		pLayer = CLayer::Create();
		pLayer->SetTag(_pLayerName);
	}
	return pLayer;
}

void CScene::Remove_Layer(const _char* _pLayerName)
{
	MAP_LAYER::iterator Iter = m_mapLayer.find(_pLayerName);

	if(Iter != m_mapLayer.end()) {
		m_RemoveLayerList.push_back(Iter->second);
		Iter->second = NULL;
	}
}

RCP_LAYER CScene::Find_Layer(const _char* _pLayerName)
{
	return m_mapLayer[_pLayerName];
}

void CScene::Remove_Object(const _char* _pLayerName, const RCP_GAMEOBJECT _pObj)
{
	RCP_LAYER pLayer = this->GetLayer(_pLayerName);
	if(pLayer) pLayer->Remove_GameObject(_pObj);
}