#include "stdafx.h"
#include "BossAI.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Boss.h"
#include "Transform.h"
#include "DetectRect.h"
#include "HpBar.h"

_USING(Client)

BossAI::BossAI(PDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
	, m_pMathMgr(Engine::MathMgr::GetInstance())
	, m_pBulletMgr(Client::BulletMgr::GetInstance())
	, m_pSoundMgr(Engine::SoundMgr::GetInstance())
{
	m_pMathMgr->AddRef();
	m_pSoundMgr->AddRef();
}


BossAI::BossAI(const BossAI & rhs)
	: Component(rhs)
	, m_pPlayerInfo(rhs.m_pPlayerInfo)
	, m_pBossInfo(rhs.m_pBossInfo)
	, m_pMathMgr(rhs.m_pMathMgr)
	, m_pBulletMgr(rhs.m_pBulletMgr)
	, m_pSoundMgr(rhs.m_pSoundMgr)
{
	m_pMathMgr->AddRef();
	m_pSoundMgr->AddRef();
}

HRESULT BossAI::Init(SUPER_CUBE_INFO* pSuperCubeInfo)
{
	if (pSuperCubeInfo == nullptr)
		return E_FAIL;

	m_pBossInfo = pSuperCubeInfo;

	return NOERROR;
}

HRESULT BossAI::SetPlayer()
{
	Player* pPlayerObj = (Player*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Player");
	if (pPlayerObj == nullptr)
		return E_FAIL;

	m_pPlayerInfo = pPlayerObj->Get_SuperCubeInfo();
	if (m_pPlayerInfo == nullptr)
		return E_FAIL;

	return NO_ERROR;
}

HRESULT BossAI::SetBoss(Boss* pBossObj)
{
	if (pBossObj == nullptr)
		return E_FAIL;

	m_pBossObj = pBossObj;

	return NOERROR;
}

HRESULT BossAI::SetAnimMgr(AnimMgr * pAnimMgr)
{
	if (pAnimMgr != nullptr)
	{
		m_pAnimMgr = pAnimMgr;
		m_pAnimMgr->AddRef();
	}
	else
		return E_FAIL;

	return NO_ERROR;
}

HRESULT BossAI::SetDetectRect(DetectRect * pDetectRect)
{
	if (pDetectRect != nullptr)
	{
		m_pDetectRect = pDetectRect;
		m_pDetectRect->AddRef();

		m_pDetectColor = m_pDetectRect->Get_PlayerDetect();
	}
	else
		return E_FAIL;

	return NO_ERROR;
}

HRESULT BossAI::SetHpBar(HpBar * pHpBar)
{
	if (pHpBar != nullptr)
	{
		m_pHpBar = pHpBar;
	}
	else
		return E_FAIL;

	if (m_pHpBar == nullptr)
		return E_FAIL;

	return NO_ERROR;
}

void BossAI::Idle(const float timeDelta)
{
	//if(mPrePreAnimState == ANIM_STATE::ATTACK0)
	if (m_bAttack)
	{
		LastTime(2.f);
		mTimeInfo.idleTime = 1.f;
		if (mTimeInfo.bLastTime)
		{
			m_pDetectRect->SetOperation(true);
		}
	}

	// HP 0이되면 객체 파괴
	if (*m_pBossInfo->pHP <= 0)
	{
		mAnimState = ANIM_STATE::DESTROY;
		mAIMode = AI_MODE::DESTROY;
		return;
	}

	// 경계로 나가는지 체크하여 중앙으로 돌아오도록 하는 함수.
	Cal_CenterPos(timeDelta);

	if (m_bCenter == false)
	{
		m_bSearch = false;
		mAnimState = ANIM_STATE::WALK;
		mAIMode = AI_MODE::WALK;
		return;
	}

	if (mTimeInfo.moveTime + mTimeInfo.idleTime < mTimeInfo.accumulateTime)
	{
		mTimeInfo.idleTime = rand() % 3 + 2.f;
		mTimeInfo.moveTime = mTimeInfo.accumulateTime;
		mRandomDegree = rand() % 140 + 50.f;

		if (rand() % 2)
		{
			mRandomDegree *= -1;
		}

		m_pAnimMgr->Set_FrameInfo(FRAME_INFO(-1, -1, -1, true));
		m_bSearch = !m_bSearch;

		if (m_pDetectColor[DetectRect::ORANGE])
		{
			m_bSearch = true;
		}
	}


	if (m_bSearch == true)
	{
		mAnimState = ANIM_STATE::SEARCH;
		mAIMode = AI_MODE::SEARCH;
	}
	else
	{
		mAnimState = ANIM_STATE::WALK;
		mAIMode = AI_MODE::WALK;
	}
}

void BossAI::AI_Mode(const float timeDelta)
{
	if (gTerrainState != TERRAIN_STATE::STAGE1_PLAY && gTerrainState != TERRAIN_STATE::STAGE2_PLAY)
		return;


	if (mAIMode != AI_MODE::DESTROY && mAIMode != AI_MODE::LANDING && mAIMode != AI_MODE::STAND)
	{
		if (*m_pBossInfo->pHP <= 0)
		{
			mAnimState = ANIM_STATE::DESTROY;
			mAIMode = AI_MODE::DESTROY;
		}
	}



	switch (mAIMode)
	{
	case AI_MODE::SEARCH:
		Idle(timeDelta);
		if (m_bCenter)
		{
			Attack();
		}
		break;
	case AI_MODE::WALK:
		Idle(timeDelta);
		if (m_pDetectColor[DetectRect::ORANGE] && m_bCenter)
		{
			mAnimState = ANIM_STATE::SEARCH;
			mAIMode = AI_MODE::SEARCH;
		}

		if (m_bCenter)
		{
			Attack();
		}
		break;
	case AI_MODE::ATTACK0:
		ChangeSearchAnim();
		break;
	case AI_MODE::ATTACK1:
		ChangeSearchAnim();
		break;
	case AI_MODE::ATTACK2:
		ChangeSearchAnim();
		break;
	case AI_MODE::ATTACK3:
		ChangeSearchAnim();
		break;
	case AI_MODE::ATTACK4:
		ChangeSearchAnim();
		break;
	case AI_MODE::STAND:
		Idle(timeDelta);
		ChangeSearchAnim();
		break;
	case AI_MODE::DESTROY:
		ChangeSearchAnim();
		break;
	case AI_MODE::LANDING:
		ChangeSearchAnim();
		break;
	case AI_MODE::END:
		// Intro시간
		if (mTimeInfo.accumulateTime >= 3.f)
		{
			mAnimState = ANIM_STATE::LANDING;
			mAIMode = AI_MODE::LANDING;
		}

		break;

	default:
		assert(false && "Boss::AI");
		break;
	}
}

void BossAI::Cal_CenterPos(const float timeDelta)
{
	mDistance = 100.f;

	if (m_pBossInfo->pMyPos->x <= 5.f || m_pBossInfo->pMyPos->x >= 25.f || m_pBossInfo->pMyPos->z >= 25.f || m_pBossInfo->pMyPos->z <= 5.f)
	{
		m_bCenter = false;

		mAnimState = ANIM_STATE::WALK;
		mAIMode = AI_MODE::WALK;

		m_vCenterPos = D3DXVECTOR3(15.f, m_pBossInfo->pMyPos->y, 15.f);
		D3DXVECTOR3 vTargetDir = m_pMathMgr->CalTargetDir(*m_pBossInfo->pMyPos, m_vCenterPos);
		D3DXVECTOR3 vBossDir = *m_pBossInfo->pMyLook;
		D3DXVec3Normalize(&vBossDir, &vBossDir);

		float degree = acosf(D3DXVec3Dot(&vTargetDir, &vBossDir));

		if (D3DXToDegree(degree) >= 5.f)
		{
			m_pDetectRect->SetOperation(false);
			if (m_pDetectColor[DetectRect::ORANGE])
			{
				m_pBossInfo->pTransform->RotationUp(D3DXToRadian(mCenterDegree * 0.1f) * timeDelta);
			}
			else
			{
				m_pBossInfo->pTransform->RotationUp(D3DXToRadian(mCenterDegree) * timeDelta);
			}
		}
		else if (D3DXToDegree(degree) >= 45.f)
		{
			m_pDetectRect->SetOperation(true);
		}
		else
		{
			m_pDetectRect->SetOperation(true);
		}
	}

	if (m_bCenter == false)
	{
		mDistance = m_pMathMgr->CalDistance(*m_pBossInfo->pMyPos, m_vCenterPos);

		if (mDistance <= 10.f)
		{
			m_pDetectRect->SetOperation(true);
			m_bCenter = true;

			if (rand() % 2)
			{
				mCenterDegree *= -1.f;
			}
		}
	}
}

void BossAI::LastTime(const float time)
{
	if (mTimeInfo.bLastTime)
		return;

	if (mTimeInfo.lastTime + time < mTimeInfo.accumulateTime)
	{
		m_pAnimMgr->Set_FrameInfo(FRAME_INFO(-1, -1, -1, true));
		mTimeInfo.lastTime = mTimeInfo.accumulateTime;
		mTimeInfo.bLastTime = true;
	}
}

bool BossAI::FrameControl(const TCHAR* pName, size_t frame)
{
	if (m_pFrameInfo == nullptr || m_pAnimInfo == nullptr)
		return false;

	if (m_pFrameInfo->rotFrame >= frame)
	{
		m_pAnimMgr->Set_FrameInfo(FRAME_INFO(-1, -1, -1, false));
		mTimeInfo.lastTime = mTimeInfo.accumulateTime;
		mTimeInfo.bLastTime = false;
		return true;
	}

	return false;
}

void BossAI::Attack()
{
	if (m_pDetectColor[DetectRect::RED])
	{
		if (m_bAttack)
		{
			if (mTimeInfo.bLastTime)
			{
				SelectAttackMode();
			}
		}
		else
		{
			SelectAttackMode();
		}
	}
}

void BossAI::InitAnimState(const TCHAR * pAnimName, const TCHAR* pCubeName, const D3DXVECTOR3 vAnimSpeed, const bool bDetectOperaction)
{
	m_pDetectRect->SetOperation(bDetectOperaction);
	m_pAnimMgr->Set_InitMatrix(L"Stand");
	m_pAnimMgr->Set_AnimInfo(pAnimName);
	m_pAnimMgr->Set_AnimSpeed(vAnimSpeed.x, vAnimSpeed.y, vAnimSpeed.z);
	m_pAnimMgr->Set_FrameInfo(FRAME_INFO(0, 0, 0, true));

	Set_FrameInfo(pCubeName);
	Set_AnimInfo(pCubeName);
}

void BossAI::InitStateVariable()
{
	mTimeInfo.lastTime = mTimeInfo.accumulateTime;
	mTimeInfo.moveTime = mTimeInfo.accumulateTime;

	m_bFrameControl = false;

	m_pFrameInfo = nullptr;
	m_pAnimInfo = nullptr;
}

void BossAI::ChangeSearchAnim()
{
	if (m_pFrameInfo != nullptr)
	{
		if (m_pFrameInfo->rotFrame >= m_pAnimInfo->cubeInfo_Space.size() - 1)
		{
			mAnimState = ANIM_STATE::SEARCH;
			mAIMode = AI_MODE::SEARCH;
			m_pAnimMgr->Set_FrameInfo(FRAME_INFO(0, 0, 0, true));
		}
	}
}

DWORD BossAI::Free()
{
	Engine::Safe_Release(m_pMathMgr);
	Engine::Safe_Release(m_pAnimMgr);
	Engine::Safe_Release(m_pSoundMgr);
	//Engine::Safe_Release(m_pBulletMgr);
	Engine::Safe_Release(m_pDetectRect);
	Component::Free();
	return 0;
}


