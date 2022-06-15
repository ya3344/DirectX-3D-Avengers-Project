#ifndef GRAPHIC_DEVICE_H__
#define GRAPHIC_DEVICE_H__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL GraphicDevice final : public Base
{
public:
	_DECLARE_SINGLETON(GraphicDevice);

private:
	explicit GraphicDevice();
	virtual ~GraphicDevice() = default;

public:
	HRESULT Init(WIN_MODE mode, HWND hWnd, const WORD winSizeX, const WORD winSizeY, LPDIRECT3DDEVICE9* ppDevice);

public:
	LPDIRECT3DDEVICE9 GetDevice() const { return m_pDevice;  }

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WIN_MODE mode, HWND hWnd, const WORD winSizeX, const WORD winSizeY);

public:
	virtual DWORD Free() override;

private:
	LPDIRECT3D9 m_pSDK = nullptr;
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

_END

#endif //GRAPHIC_DEVICE_H__

