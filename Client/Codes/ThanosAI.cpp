#include "stdafx.h"
#include "ThanosAI.h"
#include "Boss.h"
#include "Transform.h"
#include "DetectRect.h"
#include "HpBar.h"
#include "CameraMgr.h"

_USING(Client)

ThanosAI::ThanosAI(LPDIRECT3DDEVICE9 pDevice)
	: BossAI(pDevice)
{
}

ThanosAI::ThanosAI(const ThanosAI & rhs)
	: BossAI(rhs)
{
}

void ThanosAI::AnimState_Change(const float timeDelta)
{
	switch (m_pHpBar->GetDeathCount())
	{
	case DEATH_COUNT::THREE_BLUE:
		if (m_pBossInfo != nullptr)
		{
			if (*m_pBossInfo->pHP <= 2.f)
			{
				mAnimState = ANIM_STATE::ATTACK4;
				mAIMode = AI_MODE::ATTACK4;
			}
		}
	}

	if (mPreAnimState != mAnimState)
	{
		InitStateVariable();

		switch (mAnimState)
		{
		case ANIM_STATE::STAND:
			InitAnimState(L"Stand", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::SEARCH:
			m_bSearch = true;
			InitAnimState(L"Search", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), true);
			break;

		case ANIM_STATE::WALK:
			m_bSearch = false;
			InitAnimState(L"Walk", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), true);
			break;

		case ANIM_STATE::ATTACK0:
			InitAnimState(L"Attack0", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK1:
			InitAnimState(L"Attack1", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK2:
			InitAnimState(L"Attack2", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK3:
			InitAnimState(L"Attack3", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK4:
			InitAnimState(L"Attack4", L"RA_Joint", D3DXVECTOR3(0.2f, 0.2f, 0.2f), false);
			break;

		case ANIM_STATE::DESTROY:
			InitAnimState(L"Destroy", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::LANDING:
			InitAnimState(L"Landing", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::END:
			InitAnimState(L"Stand", L"RA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		default:
			assert(false && "Boss::AnimState_Change");
			break;
		}


		if (mPreAnimState == ANIM_STATE::ATTACK0 || mPreAnimState == ANIM_STATE::ATTACK1
			|| mPreAnimState == ANIM_STATE::ATTACK2 || mPreAnimState == ANIM_STATE::ATTACK3
			|| mPreAnimState == ANIM_STATE::ATTACK4)
		{
			mAttackAnimState = mPreAnimState;
			m_bAttack = true;
			mTimeInfo.bLastTime = false; // Debug
			m_pDetectRect->SetOperation(false);
		}
		else
		{
			m_bAttack = false;
		}
		mPreAnimState = mAnimState;
	}
}

void ThanosAI::AnimUpdate(const float timeDelta)
{
	mTimeInfo.accumulateTime += timeDelta;
	mAttackTime += timeDelta;
	//cout << mAccumulateTime << endl;



	switch (mAnimState)
	{
	case ANIM_STATE::WALK:
		m_pBossInfo->pTransform->GoStraight(3.f * timeDelta);
		break;

	case ANIM_STATE::STAND:
		break;

	case ANIM_STATE::SEARCH:
		if (m_bCenter == false)
			return;

		if (m_pDetectColor[DetectRect::ORANGE])
		{
			m_pBossInfo->pTransform->RotationUp(D3DXToRadian(mRandomDegree * 0.1f) * timeDelta);
			mTimeInfo.idleTime += timeDelta;
		}
		else
		{
			m_pBossInfo->pTransform->RotationUp(D3DXToRadian(mRandomDegree) * timeDelta);
		}
		break;

	case ANIM_STATE::ATTACK0:
		if (m_pFrameInfo != nullptr)
		{
			//DH_SoundMgr_Setting_180706 Setting Sound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"illusionblade_cast.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 4, L"supersuplex_crash.ogg", Engine::SoundMgr::EFFECT);
			if (m_pFrameInfo->rotFrame >= 5)
			{
				if (mAttackTime >= 0.1f)
				{
					for (int i = 0; i < 15; ++i)
					{
						m_pBulletMgr->Add_SearchBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_SearchBullet", 19, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), m_pPlayerInfo);
					}

					mAttackTime = 0.f;
				}
			}
		}
		break;

	case ANIM_STATE::ATTACK1:
		if (m_pFrameInfo != nullptr)
		{
			//DH_SoundMgr_Setting_180706 Setting Sound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"nenwha_explosion_ready.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 13, L"comet_blade_finish.ogg", Engine::SoundMgr::EFFECT);

			if (m_pFrameInfo->rotFrame >= 13)
			{
				if (mAttackTime >= 0.3f)
				{
					for (int i = 0; i < 8; ++i)
					{
						m_pBulletMgr->Add_SearchBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_SearchBullet", 20, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), m_pPlayerInfo);
					}
					mAttackTime = 0.f;
				}
				else if (mAttackTime < 0.1f)
				{
					//TY_TPS ZoomIn_180705_ Call ZoomIn Function
					CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
				}
			}
		}

		break;

	case ANIM_STATE::ATTACK2:
		if (m_pFrameInfo != nullptr)
		{
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 10, L"stonejump.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 11, L"bomb_03.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 12, L"bomb_06.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 13, L"bomb_03.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 14, L"bomb_06.ogg", Engine::SoundMgr::EFFECT);

			if (m_pFrameInfo->rotFrame >= 10)
			{
				if (mAttackTime >= 0.1f)
				{
					for (int i = 0; i < 16; ++i)
					{
						m_pBulletMgr->AddBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_MeteorBullet", 22, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()));
					}

					mAttackTime = 0.f;
				}

				if (m_pFrameInfo->rotFrame == 11)
				{
					EffectMgr::GetInstance()->EffectOperation(L"EggHit_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
						D3DXVECTOR3(0.f, -1.f, 0.f), D3DXVECTOR3(10.f, 10.f, 10.f)));
				}
			}
		}
		break;

	case ANIM_STATE::ATTACK3:
		if (m_pFrameInfo != nullptr)
		{
			//DH_SoundMgr_Setting_180706 Setting Sound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"pfireworks.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 3, L"stonejump.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 12, L"3rd_strong_lowkick_hit.ogg", Engine::SoundMgr::EFFECT);

			if (m_pFrameInfo->rotFrame >= 12)
			{
				// TY_FixBullet_180706_ Change Bullet
				if (mAttackTime >= 0.1f)
				{
					for (int i = 0; i < 10; ++i)
					{
						m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 21, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle() + i*36.f), -1);
					}

					mAttackTime = 0.f;
				}
				EffectMgr::GetInstance()->EffectOperation(L"Dust_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
					D3DXVECTOR3(0.f, -1.f, 0.f), D3DXVECTOR3(10.f, 10.f, 10.f)));

			}
			else if(m_pFrameInfo->rotFrame <= 6)
			{
				//TY_TPS ZoomIn_180705_ Call ZoomIn Function
				D3DXVECTOR3 vPos = *m_pBossInfo->pMyPos;
				vPos.y += 18.f;
				CameraMgr::GetInstance()->ZoomCamera_TPS(vPos);
			}
		}
		break;

	case ANIM_STATE::ATTACK4:
		if (m_pFrameInfo != nullptr)
		{
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 7, L"hold_go_finger.ogg", Engine::SoundMgr::EFFECT);
			CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
			if (m_pFrameInfo->rotFrame == 9)
			{
				EffectMgr::GetInstance()->EffectOperation(L"Explosion_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
					D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(32.f, 32.f, 32.f)));
				EffectMgr::GetInstance()->EffectOperation(L"Explosion2_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
					D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(25.f, 25.f, 25.f)));
			}
			else if (m_pFrameInfo->rotFrame >= 10)
			{
				gTerrainState = TERRAIN_STATE::TERRAIN_END;
			}
			
		}
		if (m_pFrameInfo->rotFrame >= 9)
		{
			m_pSoundMgr->StopAll();
		}
		break;

	case ANIM_STATE::DESTROY:

		if (m_pFrameInfo != nullptr)
		{
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 2, L"bomb_06.ogg", Engine::SoundMgr::EFFECT);

			if (m_pFrameInfo->rotFrame == 2)
			{
				DestroyPart();
			}
			//TY_TPS ZoomIn_180705_ Call ZoomIn Function
			CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
		}

		break;

	case ANIM_STATE::LANDING:
		break;

	case ANIM_STATE::END:
		break;

	default:
		assert(false && "BossAI::AnimUpdate");
		break;
	}
}


void ThanosAI::SelectAttackMode()
{
	switch (m_pHpBar->GetDeathCount())
	{
	case DEATH_COUNT::ONE_GREEN:
		if (mAttackAnimState == ANIM_STATE::ATTACK3)
		{
			mAnimState = ANIM_STATE::ATTACK0;
			mAIMode = AI_MODE::ATTACK0;
		}
		else
		{
			mAnimState = ANIM_STATE::ATTACK3;
			mAIMode = AI_MODE::ATTACK3;
		}
		break;
	case DEATH_COUNT::TWO_YELLOW:
		if (mAttackAnimState == ANIM_STATE::ATTACK2)
		{
			mAnimState = ANIM_STATE::ATTACK1;
			mAIMode = AI_MODE::ATTACK1;
		}
		else
		{
			mAnimState = ANIM_STATE::ATTACK2;
			mAIMode = AI_MODE::ATTACK2;
		}
		break;
	case DEATH_COUNT::THREE_BLUE:
		if (mAttackAnimState == ANIM_STATE::ATTACK3)
		{
			mAnimState = ANIM_STATE::ATTACK2;
			mAIMode = AI_MODE::ATTACK2;
		}
		else
		{
			mAnimState = ANIM_STATE::ATTACK3;
			mAIMode = AI_MODE::ATTACK3;
		}

		if (m_pBossInfo != nullptr)
		{
			if (*m_pBossInfo->pHP <= 2.f)
			{
				mAnimState = ANIM_STATE::ATTACK4;
				mAIMode = AI_MODE::ATTACK4;
			}
		}
		break;
	case DEATH_COUNT::DEATH_END:
		break;
	default:
		assert(false && "ThanosAI::ChangeAttackAnim");
		break;
	}
}

void ThanosAI::DestroyPart()
{
	if (*m_pBossInfo->pHP <= 0.f)
	{
		switch (m_pHpBar->GetDeathCount())
		{
		case DEATH_COUNT::ONE_GREEN:
			EffectMgr::GetInstance()->EffectOperation(L"Explosion_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
				D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(7.f, 7.f, 7.f)));

			m_pAnimMgr->Delete_HierachyCube(L"L_Hand");
			break;
		case DEATH_COUNT::TWO_YELLOW:

			EffectMgr::GetInstance()->EffectOperation(L"Explosion_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
				D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(7.f, 7.f, 7.f)));

			m_pAnimMgr->Delete_HierachyCube(L"L_Arm");
			break;
		case DEATH_COUNT::THREE_BLUE:
			mAnimState = ANIM_STATE::ATTACK4;
			mAIMode = AI_MODE::ATTACK4;
			m_pAnimMgr->DeleteAllCube();
			break;
		default:
			assert(false && "BossAI::DestroyPart");
			break;
		}

		m_pHpBar->AddDeathCount();

		*m_pBossInfo->pHP = *m_pBossInfo->pMaxHP;
		m_pHpBar->SetHp(*m_pBossInfo->pHP);
#ifdef _DEBUG
#ifdef _AI_DEBUG
		cout << "BossHP :" << *m_pBossInfo->pHP << endl;
#endif
#endif
	}
}

ThanosAI * ThanosAI::Create(LPDIRECT3DDEVICE9 pDevice, SUPER_CUBE_INFO * pSuperCubeInfo)
{
	ThanosAI* pInstance = new ThanosAI(pDevice);

	if (FAILED(pInstance->Init(pSuperCubeInfo)))
	{
		_MSG_BOX("ThanosAI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::Component * ThanosAI::Clone()
{
	return new ThanosAI(*this);
}

DWORD ThanosAI::Free()
{
	BossAI::Free();
	return 0;
}
