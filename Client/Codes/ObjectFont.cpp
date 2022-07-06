#include "stdafx.h"
#include "ObjectFont.h"

CObjectFont::CObjectFont()
:m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
{
}

CObjectFont::~CObjectFont()
{
}

HRESULT CObjectFont::Init()
{
	this->AddComponent<Engine::CRenderer>()->SetRenderType(Engine::RENDER_UI);

	m_pFont = Engine::CFontManager::Inst().FindFont("±¼¸²");

	m_pFont->SetFont("", _vec2());

	return S_OK;
}

void CObjectFont::Render()
{
	static _matrix matWorld;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pFont->Render();
}

