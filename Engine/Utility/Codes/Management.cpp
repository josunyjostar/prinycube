#include "Scene.h"
using namespace Engine;

CScene::CScene()
{
}

CScene::~CScene()
{
	this->Clear_Scene();
}

void CScene::Update_Scene()
{
	list<RCP_GAMEOBJECT>::iterator Iter = m_GameObjectList.begin();
	list<RCP_GAMEOBJECT>::iterator EndIter = m_GameObjectList.end();

	while(Iter != EndIter) {
		(*Iter)->Update();
		Iter++;
	}
}

void CScene::Render_Scene()
{
	list<RCP_GAMEOBJECT>::iterator Iter = m_GameObjectList.begin();
	list<RCP_GAMEOBJECT>::iterator EndIter = m_GameObjectList.end();

	while(Iter != EndIter) {
		(*Iter)->Render();
		Iter++;
	}
}

void CScene::Clear_Scene()
{
	m_GameObjectList.clear();
}