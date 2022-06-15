#include "InputDevice.h"


_USING(Engine)
_IMPLEMENT_SINGLETON(InputDevice)

InputDevice::InputDevice()
{
}



HRESULT InputDevice::Init(HINSTANCE hInst, HWND hWnd)
{
	// m_pInput_SDK 를 생성한다.
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(ReadyKeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(ReadyMouse(hWnd)))
		return E_FAIL;

	return NOERROR;
}

void InputDevice::SetUp_DeviceState()
{
	if (m_pKeyBoard == nullptr || m_pMouse == nullptr)
		return;

	m_pKeyBoard->GetDeviceState(256, m_KeyState);

	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);

}

HRESULT InputDevice::ReadyKeyBoard(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard); // 표준키보드에 대한 정보를 셋.

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY); // 키보드의 협조레벨을 설정한다.

	m_pKeyBoard->Acquire(); // 실제 키보드가 동작할 수 있도록 활성화한다.

	return NOERROR;
}

HRESULT InputDevice::ReadyMouse(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse); // 표준키보드에 대한 정보를 셋.

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY); // 키보드의 협조레벨을 설정한다.

	m_pMouse->Acquire(); // 실제 키보드가 동작할 수 있도록 활성화한다.

	return NOERROR;
}

bool InputDevice::MouseLButton()
{
	return (mMouseState.rgbButtons[0] & 0x80) ? true : false;
}

bool InputDevice::MouseRButton()
{
	return (mMouseState.rgbButtons[1] & 0x80) ? true : false;
}

DWORD InputDevice::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);

	return DWORD();
}


