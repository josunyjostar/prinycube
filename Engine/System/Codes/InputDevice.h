#ifndef InputDevice_h__
#define InputDevice_h__

#include "Engine_Include.h"
#include "TemplateSingleton.h"

#define DIRECTINPUT_VERSION		0x0800
#include "dinput.h"
#pragma comment(lib, "dinput8.lib")

BEGIN(Engine)

class ENGINE_DLL CInputDevice : public TSingleton<CInputDevice>
{
private:
	explicit CInputDevice();
	virtual ~CInputDevice();

public:
	friend TSingleton;

	_byte	GetKeyState(_ubyte _byKeyID) const		{ return m_byKeyState[_byKeyID]; }
	_byte	GetMouseState(_ubyte _byMouseID) const	{ return m_MouseState.rgbButtons[_byMouseID]; }
	_vec3	GetMouseMove() const					{ return _vec3(float(m_MouseState.lX), float(m_MouseState.lY), float(m_MouseState.lZ));}

public:
	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void	Update();

private:
	LPDIRECTINPUT8			m_pInputSDK;
	LPDIRECTINPUTDEVICE8	m_pKeyboard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_MouseState;

public:
	void Release();
};

END


#endif // InputDevice_h__
