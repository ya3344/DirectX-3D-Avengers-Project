#include "CubeTex.h"

_USING(Engine)

CubeTex::CubeTex(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{

}

CubeTex::CubeTex(const CubeTex & rhs)
	: VIBuffer(rhs)
{

}

HRESULT CubeTex::InitBuffer()
{
	mVtxInfo = VTX_INFO(sizeof(VTX_NORMAL_CUBE), 24, VTXFVF_NORAML_CUBE);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), 12, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTX_NORMAL_CUBE* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	// -z
	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);

	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[1].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);

	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[2].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);

	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);

	// +z
	pVertex[4].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vNormal = D3DXVECTOR3(0.f, 0.f, 1.f);

	pVertex[5].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[5].vNormal = D3DXVECTOR3(0.f, 0.f, 1.f);

	pVertex[6].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[6].vNormal = D3DXVECTOR3(0.f, 0.f, 1.f);

	pVertex[7].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vNormal = D3DXVECTOR3(0.f, 0.f, 1.f);

	// -x
	pVertex[8].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[8].vNormal = D3DXVECTOR3(-1.f, 0.f, 0.f);

	pVertex[9].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[9].vNormal = D3DXVECTOR3(-1.f, 0.f, 0.f);

	pVertex[10].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[10].vNormal = D3DXVECTOR3(-1.f, 0.f, 0.f);

	pVertex[11].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[11].vNormal = D3DXVECTOR3(-1.f, 0.f, 0.f);

	// +x
	pVertex[12].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[12].vNormal = D3DXVECTOR3(1.f, 0.f, 0.f);

	pVertex[13].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[13].vNormal = D3DXVECTOR3(1.f, 0.f, 0.f);

	pVertex[14].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[14].vNormal = D3DXVECTOR3(1.f, 0.f, 0.f);

	pVertex[15].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[15].vNormal = D3DXVECTOR3(1.f, 0.f, 0.f);

	// -y
	pVertex[16].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[16].vNormal = D3DXVECTOR3(0.f, -1.f, 0.f);

	pVertex[17].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[17].vNormal = D3DXVECTOR3(0.f, -1.f, 0.f);

	pVertex[18].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[18].vNormal = D3DXVECTOR3(0.f, -1.f, 0.f);

	pVertex[19].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[19].vNormal = D3DXVECTOR3(0.f, -1.f, 0.f);

	// +y
	pVertex[20].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[20].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);

	pVertex[21].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[21].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);

	pVertex[22].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[22].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);

	pVertex[23].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[23].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);

	for (DWORD i = 0; i < mVtxInfo.vtxNum; ++i)
		pVertex[i].vTex = pVertex[i].vPos;

	// 정점 위치 저장 다른 클래스에서 쓰기 위함
	Set_VtxPos(pVertex);

	mVtxInfo.pVB->Unlock();

	POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);

	// -z
	pIndex[0]._1 = 0; pIndex[0]._2 = 1; pIndex[0]._3 = 2;
	pIndex[1]._1 = 0; pIndex[1]._2 = 2; pIndex[1]._3 = 3;

	// +z
	pIndex[2]._1 = 6; pIndex[2]._2 = 5; pIndex[2]._3 = 4;
	pIndex[3]._1 = 7; pIndex[3]._2 = 6; pIndex[3]._3 = 4;

	// -x
	pIndex[4]._1 = 8; pIndex[4]._2 = 9; pIndex[4]._3 = 10;
	pIndex[5]._1 = 8; pIndex[5]._2 = 10; pIndex[5]._3 = 11;

	// +x
	pIndex[6]._1 = 14; pIndex[6]._2 = 13; pIndex[6]._3 = 12;
	pIndex[7]._1 = 15; pIndex[7]._2 = 14; pIndex[7]._3 = 12;

	// -y
	pIndex[8]._1 = 18; pIndex[8]._2 = 17; pIndex[8]._3 = 16;
	pIndex[9]._1 = 19; pIndex[9]._2 = 18; pIndex[9]._3 = 16;

	// +y
	pIndex[10]._1 = 20; pIndex[10]._2 = 21; pIndex[10]._3 = 22;
	pIndex[11]._1 = 20; pIndex[11]._2 = 22; pIndex[11]._3 = 23;

	mIdxInfo.pIB->Unlock();

	return NOERROR;
}

CubeTex * CubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CubeTex* pInstance = new CubeTex(pDevice);

	if (FAILED(pInstance->InitBuffer()))
	{
		_MSG_BOX("CubeTex Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * CubeTex::Clone()
{
	return new CubeTex(*this);
}

DWORD CubeTex::Free()
{
	VIBuffer::Free();
	return 0;
}
