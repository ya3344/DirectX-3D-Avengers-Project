#ifndef RECT_TEX_H__
#define RECT_TEX_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL RectTex final : public VIBuffer
{
private:
	explicit RectTex(LPDIRECT3DDEVICE9 pDevice);
	explicit RectTex(const RectTex& rhs);
	virtual ~RectTex() = default;

public:
	HRESULT InitBuffer();

public:
	static RectTex* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};


_END
#endif // RECT_TEX_H__

