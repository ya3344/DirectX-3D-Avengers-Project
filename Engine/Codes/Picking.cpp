#include "Picking.h"
#include "VIBuffer.h"

_USING(Engine)

Picking::Picking(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
{
}

Picking::Picking(const Picking & rhs)
	:Component(rhs)
{
}

HRESULT Picking::Init()
{
	return NOERROR;
}

void Picking::Cal_PickingRay(size_t mouseX, size_t mouseY, const D3DXMATRIX& matWorld)
{
	float pointX = 0.f;
	float pointY = 0.f;
	D3DXVECTOR4 vTempDir = { 0.f, 0.f, 0.f, 0.f };

	D3DVIEWPORT9 viewPort;
	m_pDevice->GetViewport(&viewPort);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &mRayMatrix.matProj);
	D3DXMatrixInverse(&mRayMatrix.matInvProj, nullptr, &mRayMatrix.matProj);

	m_pDevice->GetTransform(D3DTS_VIEW, &mRayMatrix.matView);
	D3DXMatrixInverse(&mRayMatrix.matInvView, nullptr, &mRayMatrix.matView);

	// ViewSpace -> ProjSpace
	pointX = ((2.0f * mouseX) / viewPort.Width) - 1.0f;
	pointY = ((-2.0f * mouseY) / viewPort.Height) + 1.0f;

	// ProjSapce -> ViewSpace
	mRayInfo.vStartPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	/*mRay.vDir.x = pointX / matProj._11;
	mRay.vDir.y = pointY / matProj._22;
	mRay.vDir.z = 1.0f;*/

	vTempDir = D3DXVECTOR4(pointX, pointY, 0, 1);
	D3DXVec4Transform(&vTempDir, &vTempDir, &mRayMatrix.matInvProj);
	mRayInfo.vDir = D3DXVECTOR3(vTempDir.x, vTempDir.y, vTempDir.z);

	// ViewSpace -> World
	D3DXVec3TransformCoord(&mRayInfo.vStartPos, &mRayInfo.vStartPos, &mRayMatrix.matInvView);
	D3DXVec3TransformNormal(&mRayInfo.vDir, &mRayInfo.vDir, &mRayMatrix.matInvView);

	// World -> Local
	D3DXMatrixInverse(&mRayMatrix.matInvWorld, nullptr, &matWorld);

	D3DXVec3TransformCoord(&mRayInfo.vStartPos, &mRayInfo.vStartPos, &mRayMatrix.matInvWorld);
	D3DXVec3TransformNormal(&mRayInfo.vDir, &mRayInfo.vDir, &mRayMatrix.matInvWorld);
	D3DXVec3Normalize(&mRayInfo.vDir, &mRayInfo.vDir);

	//cout << "광선방향 픽킹 = " << mRay.vDir.x << " " << mRay.vDir.y << " " << mRay.vDir.z << endl;
	//cout << "광선시작포스 픽킹 = " << mRay.vStartPos.x << " " << mRay.vStartPos.y << " " << mRay.vStartPos.z << endl;
}

bool Picking::IsPicking(VIBuffer * pBuffer, D3DXVECTOR3* pPickedPos)
{
	BOOL bCheck = false;
	const D3DXVECTOR3* pVtxPos = pBuffer->Get_VtxPos();

	if (pVtxPos == nullptr)
		return false;

	size_t vtxNumX = pBuffer->Get_VtxInfo().vtxNumX;
	size_t vixNumZ = pBuffer->Get_VtxInfo().vtxNumZ;
	size_t index = 0;

	float U = 0.f, V = 0.f, distance = 0.f;

	for (size_t z = 0; z < vixNumZ - 1; ++z)
	{
		for (size_t x = 0; x < vtxNumX - 1; ++x)
		{
			index = z * vtxNumX + x;

			bCheck = D3DXIntersectTri(&pVtxPos[(index + vtxNumX)],
				&pVtxPos[(index + vtxNumX + 1)],
				&pVtxPos[(index + 1)],
				&mRayInfo.vStartPos, &mRayInfo.vDir, &U, &V, &distance);

			if (bCheck)
			{
				*pPickedPos = mRayInfo.vStartPos + (distance * mRayInfo.vDir);
				//cout << "상단삼각형 픽킹 = " << pPickedPos->x << " " << pPickedPos->y << " " << pPickedPos->z << endl;
				return true;
			}

			bCheck = D3DXIntersectTri(&pVtxPos[(index + vtxNumX)],
				&pVtxPos[(index + 1)],
				&pVtxPos[(index)],
				&mRayInfo.vStartPos, &mRayInfo.vDir, &U, &V, &distance);

			if (bCheck)
			{
				*pPickedPos = mRayInfo.vStartPos + (distance * mRayInfo.vDir);
				//cout << "하단삼각형 픽킹 = " << pPickedPos->x << " " << pPickedPos->y << " " << pPickedPos->z << endl;
				return true;
			}
				
		}
	}

	return false;
}

Picking * Picking::Create(LPDIRECT3DDEVICE9 pDevce)
{
	Picking* pInstance = new Picking(pDevce);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Picking Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Picking * Picking::Clone()
{
	return new Picking(*this);
}

DWORD Picking::Free()
{
	Component::Free();
	return 0;
}
