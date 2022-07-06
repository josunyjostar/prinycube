#include "InputDevice.h"
using namespace Engine;

CInputDevice::CInputDevice()
{
}

CInputDevice::~CInputDevice()
{
	this->Release();
}

HRESULT CInputDevice::Init(HINSTANCE hInst, HWND hWnd)
{
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
	{
		ERR_MSG("InputSDK Created Failed");
		return E_FAIL;
	}

	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
	{
		ERR_MSG("KeyBoard Device Created Failed");
		return E_FAIL;
	}

	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyboard->Acquire();

	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		ERR_MSG("Mouse Device Created Failed");
		return E_FAIL;
	}

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDevice::Update()
{
	m_pKeyboard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void CInputDevice::Release()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pInputSDK);
}