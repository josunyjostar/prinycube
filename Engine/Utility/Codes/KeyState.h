#ifndef KeyState_h__
#define KeyState_h__

#include "Engine_Include.h"
#include "TemplateSingleton.h"

enum KEY_STATE
{
	KEY_NULL, KEY_DOWN, KEY_PUSH, KEY_UP
};

#define MAX_KEY_TYPE	256
#define MAX_COMMAND		8

BEGIN(Engine)

class ENGINE_DLL KeyState : public TSingleton<KeyState>
{
private:
	KeyState(void);
	~KeyState(void);

public:
	friend TSingleton;

	_vec2 MousePos() const 		{ return m_MousePos; }
	_vec2 MouseMovePos() const	{ return m_MouseMovePos; }
	int Wheel() const			{ return m_iWheel; }
	_ray MouseRay(int _iWinCX, int _iWinCY, _matrix _matProj) const;

	bool IsInput() const 		{ return m_bInput; }

	//Generic Method
	bool Is(int _VK)			{ return m_KeyData[GetKey(_VK)] != KEY_NULL;}
	bool IsPush(int _VK)		{ return m_KeyData[GetKey(_VK)] == KEY_PUSH;}
	bool IsDown(int _VK)		{ return m_KeyData[GetKey(_VK)] == KEY_DOWN;}
	bool IsUp(int _VK)			{ return m_KeyData[GetKey(_VK)] == KEY_UP;	}

	//Special Method
	bool Up(SHORT _KeyState = KEY_NULL) const		{ return m_KeyData[m_Up] == _KeyState; }
	bool Down(SHORT _KeyState = KEY_NULL) const		{ return m_KeyData[m_Down] == _KeyState; }
	bool Left(SHORT _KeyState = KEY_NULL) const		{ return m_KeyData[m_Left] == _KeyState; }
	bool Right(SHORT _KeyState = KEY_NULL) const	{ return m_KeyData[m_Right] == _KeyState; }
	bool Attack(SHORT _KeyState = KEY_NULL) const	{ return m_KeyData[m_Attack] == _KeyState; }

public:
	//Special Value
	int	m_Up;
	int	m_Down;
	int	m_Left;
	int	m_Right;
	int m_Attack;

public:
	void Init(HWND _hWnd);
	void Update(int _iWheel);
	void MousePosUpdate();

private:
	HWND			m_hWnd;
	KEY_STATE		m_KeyData[MAX_KEY_TYPE];
	bool			m_bInput;
	_vec2			m_MousePos;
	_vec2			m_MouseMovePos;
	int				m_iWheel;

	int				m_ObserveKey[MAX_KEY_TYPE];
	int				m_iSize;

private:
	int GetKey(int _VK);
};

extern KeyState* const KeyInput;

END

/*
CKeyState Key;
Update : Key.Update();

Key.Is(VK_SPACE);		== GetAsyncKeyState(VK_SPACE);
Key.IsPush(VK_SPACE);	== GetAsyncKeyState(VK_SPACE & 0x8000);
Key.IsDown(VK_SPACE);	== GetAsyncKeyState(VK_SPACE & 0x8001);
Key.IsUp(VK_SPACE);		== GetAsyncKeyState(VK_SPACE & 0x0001);
*/

#endif // KeyState_h__