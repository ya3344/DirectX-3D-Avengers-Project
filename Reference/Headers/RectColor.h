#ifndef RECT_COLOR_H__
#define RECT_COLOR_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL RectColor final : public VIBuffer
{
private:
	explicit RectColor(LPDIRECT3DDEVICE9 pDevice);
	explicit RectColor(const RectColor& rhs);
	virtual ~RectColor() = default;

public:
	HRESULT InitBuffer();

public:
	static RectColor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};


_END
#endif // RECT_COLOR_H__

