#ifndef CUBE_COLOR_H__
#define CUBE_COLOR_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL CubeColor final : public VIBuffer
{
private:
	explicit CubeColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CubeColor(const CubeColor& rhs);
	virtual ~CubeColor() = default;

public:
	HRESULT InitBuffer();
	//TY_PARTICLE_180623 Add Function To Change Cube Color
	void SetColor(DWORD cubeColor);

public:
	static CubeColor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};

_END

#endif