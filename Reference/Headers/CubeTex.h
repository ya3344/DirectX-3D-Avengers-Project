#ifndef CUBE_TEX_H__
#define CUBE_TEX_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL CubeTex final : public VIBuffer
{
private:
	explicit CubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CubeTex(const CubeTex& rhs);
	virtual ~CubeTex() = default;

public:
	HRESULT InitBuffer();

public:
	static CubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};


_END
#endif // CUBE_TEX_H__

