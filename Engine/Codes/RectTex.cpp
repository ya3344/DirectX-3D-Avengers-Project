#include "RectTex.h"

_USING(Engine)

RectTex::RectTex(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{

}

RectTex::RectTex(const RectTex & rhs)
	: VIBuffer(rhs)
{

}

HRESULT RectTex::InitBuffer()
{
	mVtxInfo = VTX_INFO(sizeof(VTXTEX), 4, VTXTEX_FVF);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), 2, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTXTEX* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

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

RectTex * RectTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	RectTex* pInstance = new RectTex(pDevice);

	if (FAILED(pInstance->InitBuffer()))
	{
		_MSG_BOX("RectTexutre Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * RectTex::Clone()
{
	return new RectTex(*this);
}

DWORD RectTex::Free(void)
{
	VIBuffer::Free();
	return 0;
}
