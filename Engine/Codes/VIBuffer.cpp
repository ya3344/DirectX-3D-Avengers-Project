#include "VIBuffer.h"

_USING(Engine)

VIBuffer::VIBuffer(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{

}

VIBuffer::VIBuffer(const VIBuffer& rhs)
	: Component(rhs)
	, mVtxInfo(rhs.mVtxInfo)
	, mIdxInfo(rhs.mIdxInfo)
	, m_pVtxPos(rhs.m_pVtxPos)
{
	mVtxInfo.pVB->AddRef();
	mIdxInfo.pIB->AddRef();
}

HRESULT VIBuffer::CreateBuffer()
{
	if (FAILED(m_pDevice->CreateVertexBuffer(mVtxInfo.vtxSize * mVtxInfo.vtxNum, 0, mVtxInfo.vtxFVF, D3DPOOL_MANAGED, &mVtxInfo.pVB, nullptr)))
	{
		_MSG_BOX("Create Vertex Buffer Failed");
		return E_FAIL;
	}
	
	if (FAILED(m_pDevice->CreateIndexBuffer(mIdxInfo.polygonSize * mIdxInfo.polygonNum, 0, mIdxInfo.idxFmt, D3DPOOL_MANAGED, &mIdxInfo.pIB, nullptr)))
	{
		_MSG_BOX("Create Index Buffer Failed");
		return E_FAIL;
	}

	m_pVtxPos = new D3DXVECTOR3[mVtxInfo.vtxNum];
	ZeroMemory(m_pVtxPos, sizeof(D3DXVECTOR3)* mVtxInfo.vtxNum);

	return NOERROR;
}

void VIBuffer::Render()
{
	if (mVtxInfo.pVB == nullptr || mIdxInfo.pIB == nullptr)
		return;

	// 그리기 위해 사용할 버텍스버퍼를 장치에 올려놓는다.
	m_pDevice->SetStreamSource(0, mVtxInfo.pVB, 0/*가장앞에서부터 그린다*/, mVtxInfo.vtxSize/*얼마만큼 건너뛰면서 그려야 하는가*/);
	m_pDevice->SetIndices(mIdxInfo.pIB);
	m_pDevice->SetFVF(mVtxInfo.vtxFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVtxInfo.vtxNum, 0, mIdxInfo.polygonNum);
}

void VIBuffer::Set_VtxPos(const void * pVtxData)
{
	if (mVtxInfo.pVB == nullptr)
		return;

	for (size_t i = 0; i < mVtxInfo.vtxNum; i++)
	{
		m_pVtxPos[i] = *(D3DXVECTOR3*)((byte*)pVtxData + (i * mVtxInfo.vtxSize));
	}
}

DWORD VIBuffer::Free()
{
	DWORD refCnt = 0;

	if (m_bClone == false)
	{
		Safe_Delete_Array(m_pVtxPos);
		m_pVtxPos = nullptr;
	}

	// 원본이 지워질때 완전히 지워지므로 여기서 refcnt가 1이상 남아있는것을 체크할 수 있다.
	if (refCnt = Safe_Release(mVtxInfo.pVB))
	{
		if (!m_bClone)
			_MSG_BOX("m_pVB Release Failed!");
	}
	if (refCnt = Safe_Release(mIdxInfo.pIB))
	{
		if (!m_bClone)
			_MSG_BOX("m_pIB Release Failed!");
	}
	Component::Free();
	return 0;
}

