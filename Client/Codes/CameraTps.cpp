#include "stdafx.h"
#include "CameraTps.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Boss.h"
#include "Transform.h"



_USING(Client)

CameraTps::CameraTps(LPDIRECT3DDEVICE9 pDevice)
	: Camera(pDevice)
{
}


CameraTps::CameraTps(const CameraTps& rhs)
	: Camera(rhs)
{
}

HRESULT CameraTps::Init()
{
	return NOERROR;
}

HRESULT CameraTps::Init(const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	if (FAILED(Camera::Init(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
		return E_FAIL;

	if (!m_pDevice)
	{
		_MSG_BOX("Get Device Failed");
	}

	mPace = 2.f;

	mViewInfo.vAt = D3DXVECTOR3{ 15.f, 0.f, 15.f };
	mViewInfo.vEye = D3DXVECTOR3{ 15.f, 20.f, -5.f };
	mYCriteria = 0.f;

	mZoomCount_Max = 200;
	mZoomTIme = 0.f;

	bStageInit = true;

	return NOERROR;
}

// JH_CameraTpsTarget_1800615 Player TargetPos Logic Confirm
UPDATE CameraTps::Update(const float timeDelta)
{
	if (m_pBossInfo == nullptr && m_pPlayerInfo == nullptr)
	{
		SetInfo();
	}

	mTimeDelta = timeDelta;

	//TY_ChangeStage_180705_Shake Camera
	if (m_bShake == true)
		return UPDATE::NORMAL;

	//TY_TPS_180623 Modify To Flexible Camera
	mXControl = m_pPlayerInfo->pMyPos->x;
	mZControl = m_pPlayerInfo->pMyPos->z;

	if (mXControl < 10.f)
	{
		mXControl = 10.f;
	}
	else if (mXControl > 17.f)
	{
		mXControl = 17.f;
	}

	if (mZControl < 8.f)
	{
		mZControl = 8.f;
	}
	else if (mZControl > 18.f)
	{
		mZControl = 18.f;
	}

	D3DXVECTOR3 vDir = *m_pBossInfo->pMyPos - *m_pPlayerInfo->pMyPos;
	float lenDir = D3DXVec3Length(&vDir);
	if (lenDir < 7.f)
	{
		lenDir = 7.f;
	}
	else if (lenDir > 15.f)
	{
		lenDir = 15.f;
	}

	mYControl = -15.f + lenDir*4.f;
	if (mYControl < 5.f)
	{
		mYControl = 5.f;
	}
	else if (mYControl > 20.f)
	{
		mYControl = 20.f;
	}

	if (gTerrainState == TERRAIN_STATE::STAGE2_PLAY)
	{
		mYCriteria = -30.f;
	}
	mYControl += mYCriteria;

	m_vDestAt = D3DXVECTOR3{ mXControl, m_pPlayerInfo->pMyPos->y, mZControl };
	m_vDestEye = D3DXVECTOR3{ mXControl, mYControl, -10.f + 90.f / lenDir };

	//TY_TPS ZoomIn_180705_ ZoomIn, ZoomOut
	// º¸½º Æ¯Á¤ µ¿ÀÛ ½Ã ÁÜ ÀÎ, ¾Æ¿ô
	if (m_bZoom)
	{
		m_vDestEye = D3DXVECTOR3{ m_vZoom.x, m_vZoom.y + 5.f, m_vZoom.z - 5.f };
		m_vDestAt = D3DXVECTOR3{ m_vZoom.x, m_vZoom.y, m_vZoom.z };
	}

	// ºÒ·¿Å¸ÀÓ ±â´É »ç¿ëÇßÀ» ¶§ ÁÜ ÀÎ,¾Æ¿ô
	if (m_pKeyMgr->KeyPressing(DIK_B))
	{
		mZoomTIme +=  timeDelta;
		m_vDestEye = D3DXVECTOR3{ 15.f, 30.f + mYCriteria, 13.f };
		m_vDestAt = D3DXVECTOR3{ 15.f, -20.f + mYCriteria, 15.f };
		if (mZoomTIme < 0.1f)
		{
			m_vDestEye = D3DXVECTOR3{ 15.f, 10.f + mYCriteria, 15.f };
			mPace += 100.f*timeDelta;
		}
		else if (mZoomTIme < 0.15f)
		{
			if (mPace > 0)
			{
				mPace -= 130.f * timeDelta;
			}
		}
	}
	else
	{
		mZoomTIme = 0.f;
		mPace = 2.f;
	}

	MoveToDest();
	m_vZoom = { 0.f, 0.f, 0.f };
	m_bZoom = false;

	return UPDATE::NORMAL;
}

UPDATE CameraTps::LastUpdate(const float timeDelta)
{
	//TY_ChangeStage_180705_ Change Camera Pos One Time
	if (gTerrainState == TERRAIN_STATE::STAGE2_PLAY && bStageInit)
	{
		mViewInfo.vAt = { 15.f, -30.f, 15.f };
		bStageInit = false;
	}

	Camera::LastUpdate(timeDelta);
	return UPDATE::NORMAL;
}

HRESULT CameraTps::SetInfo()
{
	Player* pPlayerObj = (Player*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Player");
	Boss* pBossObj = (Boss*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Boss");

	if (pPlayerObj == nullptr || pBossObj == nullptr)
		return E_FAIL;

	m_pPlayerInfo = pPlayerObj->Get_SuperCubeInfo();
	m_pBossInfo = pBossObj->Get_SuperCubeInfo();

	return NO_ERROR;
}

void CameraTps::SetZoom(const D3DXVECTOR3 & vZoom)
{
	m_vZoom = vZoom;
	m_bZoom = true;
}

void CameraTps::ZoomIn(const float timeDelta)
{
	D3DXVECTOR3 vDir = mViewInfo.vAt - mViewInfo.vEye;
	D3DXVec3Normalize(&vDir, &vDir);

	if (mZoomCount < mZoomCount_Max)
	{
		m_vZoom += vDir*mTimeDelta*2.f;
		++mZoomCount;
	}
}

void CameraTps::ZoomOut(const float timeDelta)
{
	if (mZoomCount > 0)
	{
		D3DXVECTOR3 vDir = mViewInfo.vAt - mViewInfo.vEye;
		D3DXVec3Normalize(&vDir, &vDir);

		float len = D3DXVec3Length(&m_vZoom);

		m_vZoom -= (vDir*mTimeDelta*2.f);
		--mZoomCount;
	}
}

//TY_TPS_180704 Function To Move Camera To Dest
void CameraTps::MoveToDest()
{
	float interEye = D3DXVec3Length(&(m_vDestEye - mViewInfo.vEye));

	if (interEye > 0.1f)
	{
		mViewInfo.vEye += ((m_vDestEye - mViewInfo.vEye)) * mTimeDelta* mPace;
	}
	//mViewInfo.vEye.x = m_vDestEye.x;

	float interAt = D3DXVec3Length(&(m_vDestAt - mViewInfo.vAt));

	if (interAt > 0.1f)
	{
		mViewInfo.vAt += ((m_vDestAt - mViewInfo.vAt)) * mTimeDelta * mPace;
	}
}

CameraTps * CameraTps::Create(LPDIRECT3DDEVICE9 pDevce, const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	CameraTps* pInstance = new CameraTps(pDevce);

	if (FAILED(pInstance->Init(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
	{
		_MSG_BOX("CameraTps Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * CameraTps::Clone()
{
	AddRef();
	return this;
}

DWORD CameraTps::Free(void)
{
	Camera::Free();

	return DWORD();
}