#include "Transform.h"
#include "VIBuffer.h"
#include "Camera.h"
#include "ObjectMgr.h"

_USING(Engine);



Transform::Transform(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{
}

Transform::Transform(const Transform & rhs)
	: Component(rhs)
	, m_matWorld(rhs.m_matWorld)
{

}

HRESULT Transform::Init()
{
	D3DXMatrixIdentity(&m_matWorld);

	return NOERROR;
}


HRESULT Transform::SetUp_World()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	return NOERROR;
}

void Transform::SetInfo(INFO info, const D3DXVECTOR3 * pVector)
{
	memcpy(&m_matWorld.m[info][0], pVector, sizeof(D3DXVECTOR3));
}

void Transform::SetInfo(INFO info, const float X, const float Y, const float Z)
{
	m_matWorld.m[info][0] = X;
	m_matWorld.m[info][1] = Y;
	m_matWorld.m[info][2] = Z;
}

const D3DXVECTOR3 * Transform::GetInfo(INFO info)
{
	return (D3DXVECTOR3*)&m_matWorld.m[info][0];
}

D3DXVECTOR3 Transform::GetScale()
{
	return D3DXVECTOR3(D3DXVec3Length((D3DXVECTOR3*)&m_matWorld.m[INFO_RIGHT][0])
		, D3DXVec3Length((D3DXVECTOR3*)&m_matWorld.m[INFO_UP][0])
		, D3DXVec3Length((D3DXVECTOR3*)&m_matWorld.m[INFO_LOOK][0]));
}


void Transform::SetupBillBoard()
{
	D3DXMATRIX		matView;

	ObjectMgr* pObjectMgr = nullptr;
	Camera* pCamera = (Engine::Camera*)pObjectMgr->GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Camera", 0);
	matView = pCamera->GetViewInfo().matView;
	D3DXMatrixInverse(&matView, nullptr, &matView);

	SetInfo(INFO_RIGHT, &(*(D3DXVECTOR3*)&matView.m[0][0]));
	SetInfo(INFO_UP, &(*(D3DXVECTOR3*)&matView.m[1][0]));
	SetInfo(INFO_LOOK, &(*(D3DXVECTOR3*)&matView.m[2][0]));
}

void Transform::TargetStep(D3DXVECTOR3 vTargetPos, const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *GetInfo(INFO_POSITION);

	//D3DXVec3Normalize(&vTargetPos, &vTargetPos);
	D3DXVECTOR3 vTargetDir = vTargetPos - vPosition;
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	
	vPosition += vTargetDir * speedPerSec;
	SetInfo(INFO_POSITION, &vPosition);
}

void Transform::GoStraight(const float speedPerSec)
{
	D3DXVECTOR3	vTemp;

	D3DXVECTOR3 vPosition = *GetInfo(INFO_POSITION);
	D3DXVECTOR3 vLook = *GetInfo(INFO_LOOK);

	vPosition += *D3DXVec3Normalize(&vTemp, &vLook) * speedPerSec;

	SetInfo(INFO_POSITION, &vPosition);
}

void Transform::RightStep(const float speedPerSec)
{
	D3DXVECTOR3 vTemp;

	*((D3DXVECTOR3*)&m_matWorld.m[INFO_POSITION][0]) += *D3DXVec3Normalize(&vTemp, (D3DXVECTOR3*)&m_matWorld.m[INFO_RIGHT][0]) * speedPerSec;
}

void Transform::LeftStep(const float speedPerSec)
{
	D3DXVECTOR3 vTemp;

	*((D3DXVECTOR3*)&m_matWorld.m[INFO_POSITION][0]) += *D3DXVec3Normalize(&vTemp, (D3DXVECTOR3*)&m_matWorld.m[INFO_RIGHT][0]) * speedPerSec * -1.f;
}

void Transform::SetScale(const float fX, const float fY, const float fZ)
{
	D3DXVECTOR3			vTmp;
	D3DXVec3Normalize(&vTmp, (D3DXVECTOR3*)&m_matWorld.m[INFO_RIGHT][0]);
	memcpy((D3DXVECTOR3*)&m_matWorld.m[INFO_RIGHT][0], &(vTmp * fX), sizeof(D3DXVECTOR3));

	D3DXVec3Normalize(&vTmp, (D3DXVECTOR3*)&m_matWorld.m[INFO_UP][0]);
	memcpy((D3DXVECTOR3*)&m_matWorld.m[INFO_UP][0], &(vTmp * fY), sizeof(D3DXVECTOR3));

	D3DXVec3Normalize(&vTmp, (D3DXVECTOR3*)&m_matWorld.m[INFO_LOOK][0]);
	memcpy((D3DXVECTOR3*)&m_matWorld.m[INFO_LOOK][0], &(vTmp * fZ), sizeof(D3DXVECTOR3));
}

void Transform::SetRotation(const float radian)
{
	mAngle = D3DXToDegree(radian);
	D3DXVECTOR3 vScaleInfo = GetScale();
	D3DXVECTOR3 vRight = D3DXVECTOR3(1.f, 0.f, 0.f) * vScaleInfo.x;
	D3DXVECTOR3	vLook = D3DXVECTOR3(0.f, 0.f, 1.f) * vScaleInfo.z;

	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, GetInfo(INFO_UP), radian);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	SetInfo(INFO_RIGHT, &vRight);
	SetInfo(INFO_LOOK, &vLook);
}

void Transform::RotationUp(const float radian)
{
	mAngle += D3DXToDegree(radian);
	D3DXVECTOR3 vScaleInfo = GetScale();
	D3DXVECTOR3 vRight;
	memcpy(&vRight, &m_matWorld.m[INFO_RIGHT][0], sizeof(D3DXVECTOR3));
	D3DXVECTOR3 vLook;
	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, GetInfo(INFO_UP), radian);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	SetInfo(INFO_RIGHT, &vRight);
	SetInfo(INFO_LOOK, &vLook);
}

HRESULT Transform::ComputeY_OnBuffer(Engine::VIBuffer * pBuffer)
{
	const D3DXVECTOR3* pVtxPos = pBuffer->Get_VtxPos();

	if (pVtxPos == nullptr)
		return E_FAIL;

	float interval = 1.f;

	size_t numX = pBuffer->Get_VtxInfo().vtxNumX;

	D3DXVECTOR3	vPos;
	memcpy(&vPos, &m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	size_t currentIdx = size_t(vPos.z / interval) * numX + size_t(vPos.x / interval);

	float ratioX = (vPos.x - pVtxPos[currentIdx + numX].x) / interval;
	float ratioZ = (pVtxPos[currentIdx + numX].z - vPos.z) / interval;

	size_t indices[4] = { currentIdx + numX, currentIdx + numX + 1, currentIdx + 1, currentIdx };

	if (ratioX > ratioZ) // 오른쪽 위 삼각형에 있을때.
	{
		m_matWorld.m[3][1] = pVtxPos[indices[0]].y + (pVtxPos[indices[1]].y - pVtxPos[indices[0]].y) * ratioX
			+ (pVtxPos[indices[2]].y - pVtxPos[indices[1]].y) * ratioZ + 0.5f;
	}
	else // 왼쪽 하단 삼각형안에 있을때.
	{
		m_matWorld.m[3][1] = pVtxPos[indices[0]].y + (pVtxPos[indices[3]].y - pVtxPos[indices[0]].y) * ratioZ
			+ (pVtxPos[indices[2]].y - pVtxPos[indices[3]].y) * ratioX + 0.5f;
	}

	return NOERROR;
}

Transform * Transform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Transform*	pInstance = new Transform(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Transform Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * Transform::Clone()
{
	return new Transform(*this);
}

DWORD Transform::Free(void)
{
	Component::Free();
	return 0;
}
