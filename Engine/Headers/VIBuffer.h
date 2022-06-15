#ifndef VIBUFFER_H__
#define VIBUFFER_H__

#include "Component.h"

_BEGIN(Engine)

typedef struct tagVtxInfo
{
	tagVtxInfo() {}
	explicit tagVtxInfo(const DWORD vtxSize, const DWORD vtxNum, const DWORD vtxFVF, const DWORD vtxNumX = 0, const DWORD vtxNumZ = 0)
	{
		this->vtxSize = vtxSize;
		this->vtxNum = vtxNum;
		this->vtxFVF = vtxFVF;
		this->vtxNumX = vtxNumX;
		this->vtxNumZ = vtxNumZ;
	}

	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	DWORD vtxSize = 0;
	DWORD vtxNum = 0;
	DWORD vtxNumX = 0;
	DWORD vtxNumZ = 0;
	DWORD vtxFVF = 0;

}VTX_INFO;

typedef struct tagIdxInfo
{
	tagIdxInfo() {}
	explicit tagIdxInfo(const DWORD polygonSize, const DWORD polygonNum, const D3DFORMAT idxFmt)
	{
		this->polygonSize = polygonSize;
		this->polygonNum = polygonNum;
		this->idxFmt = idxFmt;
	}

	LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
	DWORD polygonSize = 0;
	DWORD polygonNum = 0;
	D3DFORMAT idxFmt = D3DFMT_UNKNOWN;

}IDX_INFO;

class _ENGINE_DLL VIBuffer abstract : public Component
{
protected:
	explicit VIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit VIBuffer(const VIBuffer& rhs);
	virtual ~VIBuffer() = default;

public:
	HRESULT CreateBuffer();
	void Render();

public:
	void Set_VtxPos(const void* pVtxData);

public:
	VTX_INFO Get_VtxInfo() const { return mVtxInfo; }
	IDX_INFO Get_IdxInfo() const { return mIdxInfo; }
	D3DXVECTOR3* Get_VtxPos() const { return m_pVtxPos; }

public:
	virtual Component* Clone() = 0;
	virtual DWORD Free() override;

protected:
	VTX_INFO mVtxInfo;
	IDX_INFO mIdxInfo;

protected:
	D3DXVECTOR3* m_pVtxPos = nullptr;
};

_END

#endif //  VIBUFFER_H__