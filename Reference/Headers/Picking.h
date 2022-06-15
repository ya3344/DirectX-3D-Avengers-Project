#ifndef PICKING_H__
#define PICKING_H__

#include "Component.h"

_BEGIN(Engine)

class VIBuffer;

typedef struct tagRayInfo
{
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vStartPos = { 0.f, 0.f, 0.f };
}RAY_INFO;

typedef struct tagRayMatrix
{
	tagRayMatrix()
	{
		D3DXMatrixIdentity(&matProj);
		D3DXMatrixIdentity(&matInvProj);
		D3DXMatrixIdentity(&matView);
		D3DXMatrixIdentity(&matInvView);
		D3DXMatrixIdentity(&matInvWorld);
	}

	D3DXMATRIX matProj;
	D3DXMATRIX matInvProj;
	D3DXMATRIX matView;
	D3DXMATRIX matInvView;
	D3DXMATRIX matInvWorld;

}RAY_MATIRX;
	
class _ENGINE_DLL Picking final : public Component
{
private:
	explicit Picking(LPDIRECT3DDEVICE9 pDevice);
	explicit Picking(const Picking& rhs);
	virtual ~Picking() = default;

public:
	HRESULT Init();

public:
	void Cal_PickingRay(size_t mouseX, size_t mouseY, const D3DXMATRIX& matWorld);
	bool IsPicking(VIBuffer * pBuffer, D3DXVECTOR3* pPickedPos);

public:
	static Picking* Create(LPDIRECT3DDEVICE9 pDevce);
	virtual Picking* Clone() override;
	virtual DWORD Free() override;

private:
	RAY_INFO mRayInfo;
	RAY_MATIRX mRayMatrix;
};

_END

#endif

