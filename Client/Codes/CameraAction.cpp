#include "stdafx.h"
#include "CameraAction.h"
#include "CameraMgr.h"
#include "CameraTps.h"

_USING(Client)

CameraAction::CameraAction(LPDIRECT3DDEVICE9 pDevice)
	: Camera(pDevice)
{
}


CameraAction::CameraAction(const CameraAction& rhs)
	: Camera(rhs)
{
}

HRESULT CameraAction::Init()
{
	return NOERROR;
}

HRESULT CameraAction::Init(const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	if (FAILED(Camera::Init(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
		return E_FAIL;

	if (!m_pDevice)
	{
		_MSG_BOX("Get Device Failed");
	}

	// Load Path Data
	PathData::GetInstance()->LoadCameraPathData(&mPathData_Space, L"path_data_Intro");
	mPathNum = PathData::GetInstance()->GetPathDataNum();

	mMovePace = 0.1f;
	mRotPace = 0.07f;

	return NOERROR;
}

UPDATE CameraAction::Update(const float timeDelta)
{
	if (m_pKeyMgr == nullptr)
		return UPDATE::FAIL;

	FollowPath(timeDelta);

	return UPDATE::NORMAL;
}

UPDATE CameraAction::LastUpdate(const float timeDelta)
{
	Camera::LastUpdate(timeDelta);

	return UPDATE::NORMAL;
}

// TY_ChangeStage_180703_ Function To Change PathData
void CameraAction::SetPathData(TCHAR * pathName)
{
	for (PATH_DATA* pPath : mPathData_Space)
	{
		Engine::Safe_Delete(pPath);
	}
	mPathData_Space.clear();

	PathData::GetInstance()->LoadCameraPathData(&mPathData_Space, pathName);
	mPathNum = PathData::GetInstance()->GetPathDataNum();
}

void CameraAction::FollowPath(const float timeDelta)
{
	mPathUpdateTime += timeDelta;

	//TY_ChangeStage_180705_Check Index Out Of Range
	if (mPathData_Space.size() <= m_curFrame)
	{
		m_curFrame = 0;

#ifdef _DEBUG
		_MSG_BOX("CameraAction Vector Out Of Range");
#endif
		CameraMgr::GetInstance()->ChangeCamera(L"Prototype_GameObject_CameraTps");
		return;
	}

	if ( mPathNum <= m_curFrame + 1 )
	{
		m_curFrame = 0;
		CameraMgr::GetInstance()->ChangeCamera(L"Prototype_GameObject_CameraTps");
		return;
	}

	mViewInfo.vEye = mPathData_Space[m_curFrame]->vPos *(1.f - mPathUpdateTime) + mPathData_Space[m_curFrame + 1]->vPos *mPathUpdateTime;
	D3DXVECTOR3 m_vDir = mPathData_Space[m_curFrame]->vDir *(1.f - mPathUpdateTime) + mPathData_Space[m_curFrame + 1]->vDir *mPathUpdateTime;

	if (mPathUpdateTime > 1)
	{
		mPathUpdateTime = 0;
		++m_curFrame;
	}

	// 원본 Eye를 유지하기 위해 값 복사
	mViewInfo.vAt = mViewInfo.vEye + m_vDir;

	D3DXMatrixLookAtLH(&mViewInfo.matView, &mViewInfo.vEye, &mViewInfo.vAt, &mViewInfo.vAxisY);

	m_pDevice->SetTransform(D3DTS_VIEW, &mViewInfo.matView);
}

CameraAction * CameraAction::Create(LPDIRECT3DDEVICE9 pDevce, const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	CameraAction* pInstance = new CameraAction(pDevce);

	if (FAILED(pInstance->Init(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
	{
		_MSG_BOX("CameraTps Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * CameraAction::Clone()
{
	AddRef();
	return this;
}

DWORD CameraAction::Free(void)
{
	PathData::GetInstance()->DestroyInstance();

	for (PATH_DATA* pPath : mPathData_Space)
	{
		Engine::Safe_Delete(pPath);
	}
	mPathData_Space.clear();

	Camera::Free();

	return DWORD();
}
