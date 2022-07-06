#include "KeyState.h"
using namespace Engine;

KeyState::KeyState(void)
:m_iSize(0)
{
	m_ObserveKey[m_iSize++] = m_Up = VK_UP;
	m_ObserveKey[m_iSize++] = m_Down = VK_DOWN;
	m_ObserveKey[m_iSize++] = m_Left = VK_LEFT;
	m_ObserveKey[m_iSize++] = m_Right = VK_RIGHT;
	m_ObserveKey[m_iSize++] = m_Attack = 'Z';
}

KeyState::~KeyState(void)
{
}

_ray KeyState::MouseRay(int _iWinCX, int _iWinCY, _matrix _matProj) const
{
	_ray ray;
	ray.Dir.x = m_MousePos.x / _iWinCX * 2.f - 1.f;
	ray.Dir.y = m_MousePos.y / _iWinCY * 2.f + 1.f;

	ray.Dir = TransformNormal(ray.Dir, *D3DXMatrixInverse(&_matProj, NULL, &_matProj));
	ray.Dir = Normalize(ray.Dir);
	return ray;
}

void KeyState::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;
}

void KeyState::Update(int _iWheel)
{
	m_bInput = false;
	for(int i = 0; i < m_iSize; ++i) {
		SHORT state = GetAsyncKeyState(m_ObserveKey[i]);

		switch(m_KeyData[m_ObserveKey[i]]){
			case KEY_NULL:
				if(state) 	m_KeyData[m_ObserveKey[i]] = KEY_DOWN;
				break;

			case KEY_DOWN:
				if(state)	m_KeyData[m_ObserveKey[i]] = KEY_PUSH;
				else		m_KeyData[m_ObserveKey[i]] = KEY_UP;
				break;

			case KEY_PUSH:
				if(!state)	m_KeyData[m_ObserveKey[i]] = KEY_UP;
				break;

			case KEY_UP:
				if(state)	m_KeyData[m_ObserveKey[i]] = KEY_DOWN;
				else		m_KeyData[m_ObserveKey[i]] = KEY_NULL;
				break;
		}

		if(m_KeyData[m_ObserveKey[i]] != KEY_NULL) m_bInput = true;
	}

	this->MousePosUpdate();
	m_iWheel = _iWheel;
}

void KeyState::MousePosUpdate()
{
	static POINT _WinMousePoint;

	GetCursorPos(&_WinMousePoint);
	ScreenToClient(m_hWnd, &_WinMousePoint);

	m_MousePos.x = (float)_WinMousePoint.x;
	m_MousePos.y = (float)_WinMousePoint.y;

	static _vec2 _vPrevPos = m_MousePos;
	m_MouseMovePos = m_MousePos - _vPrevPos;
	_vPrevPos = m_MousePos;
}

int KeyState::GetKey(int _VK)
{
	for(int i = 0; i < m_iSize; ++i)
		if(m_ObserveKey[i] == _VK) return _VK;
	m_ObserveKey[m_iSize++] = _VK;
	return _VK;
}