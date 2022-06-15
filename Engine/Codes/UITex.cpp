#include "UITex.h"

_USING(Engine)

UITex::UITex(LPDIRECT3DDEVICE9 pGraphic_Device)
	: VIBuffer(pGraphic_Device)
{
}

UITex::UITex(const UITex & rhs)
	: VIBuffer(rhs)
{

}

HRESULT UITex::Ready_Buffer()
{
	mVtxInfo = VTX_INFO(sizeof(VTX_UI), 4, VTXFVF_UI);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), 2, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTX_UI* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR4(-0.5f, -0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR4(0.5f, -0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR4(0.5f, 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR4(-0.5f, 0.5f, 0.f, 1.f);
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

void UITex::Render_Buffer(const D3DXMATRIX* pMatrix)
{
	VTX_UI* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (size_t i = 0; i < mVtxInfo.vtxNum; ++i)
	{
		D3DXVECTOR3	vTmp;

		D3DXVec3TransformCoord(&vTmp, &m_pVtxPos[i], pMatrix);

		*((D3DXVECTOR3*)((byte*)pVertex + (i * mVtxInfo.vtxSize))) = vTmp;
	}

	mVtxInfo.pVB->Unlock();


	VIBuffer::Render();
}

UITex * UITex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	UITex*	pInstance = new UITex(pDevice);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		_MSG_BOX("UITex Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * UITex::Clone()
{
	return new UITex(*this);
}

DWORD UITex::Free(void)
{
	VIBuffer::Free();

	return DWORD();
}
