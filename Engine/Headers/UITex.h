#ifndef VIEWPORT_H__
#define VIEWPORT_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL UITex final : public VIBuffer
{
private:
	explicit UITex(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit UITex(const UITex& rhs);
	virtual ~UITex() = default;
public:
	HRESULT Ready_Buffer();
	virtual void Render_Buffer(const D3DXMATRIX* pMatrix);

public:
	static UITex* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

};
_END
#endif