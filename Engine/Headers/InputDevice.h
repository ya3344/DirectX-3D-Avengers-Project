#ifndef INPUT_DEVICE_H__
#define INPUT_DEVICE_H__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL InputDevice final : public Base
{
	_DECLARE_SINGLETON(InputDevice)

public:
	enum MOUSE_MOVE { MOVE_X, MOVE_Y, MOVE_Z };

private:
	explicit InputDevice();
	virtual ~InputDevice() = default;



public:
	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void SetUp_DeviceState();

private:
	HRESULT ReadyKeyBoard(HWND hWnd);
	HRESULT ReadyMouse(HWND hWnd);

public:
	char Get_KeyState(byte KeyIndex) const { return m_KeyState[KeyIndex]; }
	long Get_MoveState(MOUSE_MOVE type) const { return *((long*)&mMouseState + type); }
	bool MouseLButton();
	bool MouseRButton();

public:
	virtual DWORD Free(void);

private:
	LPDIRECTINPUT8 m_pInputSDK = nullptr; // 내 장치에대한 조사 AND 장치를 생성하는 작업

private:
	LPDIRECTINPUTDEVICE8 m_pMouse = nullptr;
	DIMOUSESTATE mMouseState;

private:
	LPDIRECTINPUTDEVICE8 m_pKeyBoard = nullptr;
	char m_KeyState[256] = { 0 };

};

_END

#endif // INPUT_DEVICE_H__