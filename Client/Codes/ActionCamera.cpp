#include "stdafx.h"
#include "ActionCamera.h"

CActionCamera::CActionCamera()
:m_fCamDist(5.f)
,m_fLerpDist(5.f)
{
}

CActionCamera::~CActionCamera()
{
}

void CActionCamera::Update(const _float& _fDeltaTime)
{
	CCamera::Update(_fDeltaTime);
	/*
	static _vec2 vMousePos;
	static bool bCursorNULL = false;
	if(Engine::KeyInput->IsDown(VK_LBUTTON))
	{
		vMousePos = Engine::KeyInput->MousePos();
		if(vMousePos.x > 0.f && vMousePos.x < WINCX && vMousePos.y > 0.f && vMousePos.y < WINCY)
		{
			vMousePos = _vec2(WINCX/2.f, WINCY/2.f);
			bCursorNULL = true;
			ShowCursor(NULL);

			POINT pMousePos = {long(WINCX/2.f), long(WINCY/2.f)};
			ClientToScreen(g_hWnd, &pMousePos);
			SetCursorPos(pMousePos.x, pMousePos.y);
		}
	}
	else if(bCursorNULL)
	{
		vMousePos = Engine::KeyInput->MousePos();

		m_pCamera->RotateYawPitch((vMousePos.x - WINCX/2.f)/WINCX*10.f, (vMousePos.y - WINCY/2.f)/WINCY*10.f);

		POINT pMousePos = {long(WINCX/2.f), long(WINCY/2.f)};
		ClientToScreen(g_hWnd, &pMousePos);
		SetCursorPos(pMousePos.x, pMousePos.y);

		if(Engine::KeyInput->IsDown(VK_ESCAPE)) {
			bCursorNULL = false;
			ShowCursor(true);
		}
	}
	*/

	_vec2 vMouseMovePos = Engine::KeyInput->MouseMovePos();
	this->RotateYawPitch(vMouseMovePos.x/WINCX*10.f, vMouseMovePos.y/WINCY*10.f);

	if(m_pTargetTransform)
	{
		m_fCamDist += Engine::KeyInput->Wheel();
		m_fCamDist = BETWEEN(m_fCamDist, MIN_CAM_DIST, MAX_CAM_DIST);

		m_fLerpDist = Lerp(m_fLerpDist, m_fCamDist, 0.1f);

		this->SetPosition(m_pTargetTransform->GetPosition() - this->GetLookVector()*m_fLerpDist + this->GetUpVector()*(m_fLerpDist/5.f));
	}
	else
	{
		_vec3 vMoveVector;
		if(Engine::KeyInput->Is('W'))	vMoveVector += this->GetLookVector();
		if(Engine::KeyInput->Is('S'))	vMoveVector -= this->GetLookVector();
		if(Engine::KeyInput->Is('A'))	vMoveVector -= this->GetRightVector();
		if(Engine::KeyInput->Is('D'))	vMoveVector += this->GetRightVector();

		this->SetPosition(this->GetPosition() + vMoveVector*(5.f*_fDeltaTime));
	}
	this->SetViewMatrix();
}