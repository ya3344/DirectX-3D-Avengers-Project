#include "RectColor.h"

_USING(Engine)

RectColor::RectColor(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{

}

RectColor::RectColor(const RectColor & rhs)
	: VIBuffer(rhs)
{

}

HRESULT RectColor::InitBuffer()
{
	mVtxInfo = VTX_INFO(sizeof(VTXCOL), 4, VTXCOL_FVF);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), 2, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTXCOL* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertex[0].color = D3DCOLOR_ARGB(255, 255, 255, 0);

	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertex[1].color = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertex[2].color = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVertex[3].color = D3DCOLOR_ARGB(255, 255, 0, 255);

	// 정점 위치 저장 다른 클래스에서 쓰기 위함
	Set_VtxPos(pVertex);

	mVtxInfo.pVB->Unlock();

	POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	mIdxInfo.pIB->Unlock();

	return NOERROR;
}

RectColor * RectColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	RectColor* pInstance = new RectColor(pDevice);

	if (FAILED(pInstance->InitBuffer()))
	{
		_MSG_BOX("RectColor Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * RectColor::Clone()
{
	return new RectColor(*this);
}

DWORD RectColor::Free(void)
{
	VIBuffer::Free();
	return 0;
}
