#include "stdafx.h"
#include "CameraFree.h"
#include "PathData.h"

_USING(Client)

CameraFree::CameraFree(LPDIRECT3DDEVICE9 pDevice)
	: Camera(pDevice)
{
}


CameraFree::CameraFree(const CameraFree& rhs)
	: Camera(rhs)
{
}

HRESULT CameraFree::Init()
{
	return E_NOTIMPL;
}

HRESULT CameraFree::Init(const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	if (FAILED(Camera::Init(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
		return E_FAIL;

	return NOERROR;
}

UPDATE CameraFree::Update(const float timeDelta)
{
	if (m_pKeyMgr == nullptr)
		return UPDATE::FAIL;

	float keySpeed = 20.f;
	float mouseSpeed = 6.f;
	// 전진
	if (m_pKeyMgr->KeyPressing(DIK_W))
	{
		D3DXVECTOR3 vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vLook, &vLook) * keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vLook, &vLook) * keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_S))
	{
		D3DXVECTOR3	vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vLook, &vLook) * -keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vLook, &vLook) * -keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_A))
	{
		D3DXVECTOR3	vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vRight, &vRight) * -keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vRight, &vRight) * -keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_D))
	{
		D3DXVECTOR3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vRight, &vRight) * keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vRight, &vRight) * keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_R))
	{
		mViewInfo.vEye.y += keySpeed * timeDelta;
		mViewInfo.vAt.y += keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_F))
	{
		mViewInfo.vEye.y -= keySpeed * timeDelta;
		mViewInfo.vAt.y -= keySpeed * timeDelta;
	}


	D3DXVECTOR3	vLook;
	memcpy(&vLook, &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

	long moveValue = 0;

	if (moveValue = m_pKeyMgr->MouseMove(Engine::KeyMgr::VERTICAL))
	{
		D3DXVECTOR3 vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		D3DXMATRIX matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(moveValue * timeDelta) * mouseSpeed);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		mViewInfo.vAt = mViewInfo.vEye + vLook;
	}

	if (moveValue = m_pKeyMgr->MouseMove(Engine::KeyMgr::HORIZONTAL))
	{
		D3DXMATRIX matRot;

		D3DXMatrixRotationY(&matRot, D3DXToRadian(moveValue * timeDelta) * mouseSpeed);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		mViewInfo.vAt = mViewInfo.vEye + vLook;
	}


	return UPDATE::NORMAL;
}

UPDATE CameraFree::LastUpdate(const float timeDelta)
{
	Camera::LastUpdate(timeDelta);

	//TY_CAMERAPATH_180618 Call Function MakePath
	if (m_pKeyMgr->KeyDown(DIK_NUMPAD1))
	{
		m_bRecord = true;
		cout << "녹화 시작"<< endl;
	}
	else if (m_pKeyMgr->KeyDown(DIK_NUMPAD2))
	{
		m_bRecord = false;
		cout << "녹화 끝"<< endl;
	}	
	if(m_bRecord)
		MakePath(timeDelta);

	return UPDATE::NORMAL;
}

void CameraFree::MakePath(const float timeDelta)
{
	mPathUpdateTime += timeDelta;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&vDir, &(mViewInfo.vAt - mViewInfo.vEye));

	if (mPathUpdateTime > 1)
	{
		cout << ++mRecordTime <<"초 경과"<< endl;
		mPathUpdateTime = 0;
		PathData::GetInstance()->AddCameraPathData(mViewInfo.vAt, vDir);
	}
}

CameraFree * CameraFree::Create(LPDIRECT3DDEVICE9 pDevce, const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	CameraFree* pInstance = new CameraFree(pDevce);

	if (FAILED(pInstance->Init(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
	{
		_MSG_BOX("Camera_Free Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * CameraFree::Clone()
{
	AddRef();
	return this;
}

DWORD CameraFree::Free(void)
{
	Camera::Free();

	return DWORD();
}