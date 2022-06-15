#ifndef KEYMGR_H__
#define KEYMGR_H__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL  KeyMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(KeyMgr)

private:
	explicit KeyMgr();
	virtual ~KeyMgr() = default;

public:
	enum MOUSE_BUTTON { LBUTTON, RBUTTON };
	enum MOUSE_MOVE { HORIZONTAL, VERTICAL };

public:
	bool KeyDown(int key);
	bool KeyUp(int key);
	bool KeyPressing(int key);

	bool MouseButtonDown(int buttonNum); // 0: Left Click, 1: Right Click
	bool MouseButtonUp(int buttonNum);
	bool MouseButtonPressing(int buttonNum);
	
	long MouseMove(int dir); // 0: Horizontal, 1: Vertical
	bool MouseWheelDown();
	bool MouseWheelUp();
	
public:
	HRESULT Init(HINSTANCE hInst, HWND hWnd); // Initialize
	void UpdateKeyMgr(); // Call Before KeyCheck for Every Frame

private:
	HRESULT ReadyKeyBoard(HWND hWnd);
	HRESULT ReadyMouse(HWND hWnd);

public:
	virtual DWORD Free();

private:
	static constexpr size_t KEY_MAX = 256;
	static constexpr size_t MOUSE_MAX = 4;

private:
	bool m_bKeyDown[KEY_MAX] = { 0, };
	bool m_bKeyUp[KEY_MAX] = { 0, };

	bool m_bMouseDown[MOUSE_MAX] = { 0, };
	bool m_bMouseUp[MOUSE_MAX] = { 0, }; 

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	DIMOUSESTATE			m_mouseState;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	byte					m_keyState[KEY_MAX] = { 0, };
};

_END

#endif

