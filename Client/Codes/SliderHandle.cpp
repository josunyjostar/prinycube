#include "stdafx.h"
#include "SliderHandle.h"

void CSliderHandle::Update()
{
	_vec3 vMousePos = _vec3(Engine::KeyInput->MousePos());

	vMousePos.x = (vMousePos.x / WINCX * 2.f - 1.f)*0.5f;
	vMousePos.y = (1.f - vMousePos.y / WINCY * 2.f)*0.5f;

	_vec3 vUIPos = m_pTransform->GetPosition();
	_vec3 vUISize= m_pTransform->GetScale();

	if(vMousePos.x > vUIPos.x - vUISize.x/2.f &&
		vMousePos.x < vUIPos.x + vUISize.x/2.f &&
		vMousePos.y > vUIPos.y - vUISize.y/2.f &&
		vMousePos.y < vUIPos.y + vUISize.y/2.f)
	{
		if(!m_bEnter) {
			m_bEnter = true;
			this->OnMouseEnter();			
		}
	}
	else 
	{
		if(m_bEnter) {
			m_bEnter = false;
			this->OnMouseExit();
		}
	}

	if(m_bEnter && Engine::KeyInput->IsDown(VK_LBUTTON))
	{
		m_bButtonDown = true;
		this->OnMouseButtonDown();
	}

	if(m_bButtonDown && Engine::KeyInput->IsUp(VK_LBUTTON))
	{
		m_bButtonDown = false;
		this->OnMouseButtonUp();
	}
}