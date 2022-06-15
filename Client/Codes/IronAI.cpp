#include "stdafx.h"
#include "IronAI.h"	//cout << mAccumulateTime << endl;
#include "Boss.h"
#include "Transform.h"
#include "DetectRect.h"
#include "HpBar.h"
#include "CameraMgr.h"

_USING(Client)

IronAI::IronAI(LPDIRECT3DDEVICE9 pDevice)
	: BossAI(pDevice)
{
}

IronAI::IronAI(const IronAI & rhs)
	: BossAI(rhs)
{
}

void IronAI::AnimState_Change(const float timeDelta)
{
	if (mPreAnimState != mAnimState)
	{
		InitStateVariable();

		switch (mAnimState)
		{
		case ANIM_STATE::STAND:
			InitAnimState(L"Stand", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::SEARCH:
			m_bSearch = true;
			InitAnimState(L"Search", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), true);
			break;

		case ANIM_STATE::WALK:
			m_bSearch = false;
			InitAnimState(L"Walk", L"LA_Joint", D3DXVECTOR3(1.3f, 1.3f, 1.3f), true);
			break;

		case ANIM_STATE::ATTACK0:
			InitAnimState(L"Attack0", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK1:
			InitAnimState(L"Attack1", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK2:
			InitAnimState(L"Attack2", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK3:
			InitAnimState(L"Attack3", L"LL_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::ATTACK4:
			InitAnimState(L"Attack4", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::DESTROY:
			InitAnimState(L"Destroy", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::LANDING:
			InitAnimState(L"Landing", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
			break;

		case ANIM_STATE::END:
			InitAnimState(L"Stand", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f), false);
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

void IronAI::AnimUpdate(const float timeDelta)
{
	mTimeInfo.accumulateTime += timeDelta;
	mAttackTime += timeDelta;
	//cout << mAccumulateTime << endl;
	switch (mAnimState)
	{
	case ANIM_STATE::WALK:
		m_pBossInfo->pTransform->GoStraight(3.5f * timeDelta);
		if (m_pSoundMgr != nullptr)
		{
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"hellmarch_soldiers_01.ogg", Engine::SoundMgr::BOSS);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 3, L"hellmarch_soldiers_02.ogg", Engine::SoundMgr::BOSS);
		}

		break;

	case ANIM_STATE::STAND:
		break;

	case ANIM_STATE::SEARCH:
		if (m_bCenter == false)
			return;
		Engine::SoundMgr::GetInstance()->StopSound(Engine::SoundMgr::LOOP_EFFECT);
		//DH_SoundSetting_180705_SetSound_
		if (m_pSoundMgr != nullptr)
		{
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"dimension_runner_tx_80_stand.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 5, L"hs_1_cast.ogg", Engine::SoundMgr::BOSS);
		}


		if (m_bFrameControl == false)
		{
			m_bFrameControl = FrameControl(L"LA_Joint", 7);
		}

		if (m_bFrameControl)
		{
			if (m_pDetectColor[DetectRect::ORANGE])
			{
				m_pBossInfo->pTransform->RotationUp(D3DXToRadian(mRandomDegree * 0.1f) * timeDelta);
				mTimeInfo.idleTime += timeDelta;
			}
			else
			{
				m_pBossInfo->pTransform->RotationUp(D3DXToRadian(mRandomDegree) * timeDelta);
			}
		}
		break;

	case ANIM_STATE::ATTACK0:
		if (m_bFrameControl == false)
		{
			m_bFrameControl = FrameControl(L"LA_Joint", 2);
			//TY_TPS ZoomIn_180705_ Call ZoomIn Function
			CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
		}

		if (m_bFrameControl && mTimeInfo.bLastTime == false)
		{
			LastTime(3.f);
			m_pBossInfo->pTransform->RotationUp(D3DXToRadian(140) * timeDelta);

			if (mAttackTime >= 0.1f)
			{
				m_pBulletMgr->AddBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_NormalBullet", 17, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()));
				m_pBulletMgr->AddBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_NormalBullet", 17, *m_pBossInfo->pMyPos, 180.f + *m_pBossInfo->pTransform->GetAngle()));
				mAttackTime = 0.f;
			}
			//DH_SoundSetting_180705_SetSound
			if (m_pSoundMgr != nullptr)
			{
				m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 2, L"apocal_laser_loop.ogg", Engine::SoundMgr::EFFECT);
				m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"bolt_mx_targeting02.ogg", Engine::SoundMgr::EFFECT);
				m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"robot_butting_act01.ogg", Engine::SoundMgr::EFFECT);
				m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 7, L"robot_butting_act02.ogg", Engine::SoundMgr::EFFECT);
			}

		}
		break;

	case ANIM_STATE::ATTACK1:
		if (m_pFrameInfo != nullptr)
		{
			if (m_pFrameInfo->rotFrame >= 5)
			{
				if (mAttackTime >= 0.1f)
				{
					for (int i = 0; i < 8; ++i)
					{
						m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_CircleBullet", 0, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), i);
					}

					mAttackTime = 0.f;
				}
			}
			else
			{
				//TY_TPS ZoomIn_180705_ Call ZoomIn Function
				CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
			}
			//DH_SoundSetting_180705_SetSound_SetSound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"robot_butting_act01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"bolt_mx_targeting01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 4, L"supernova_fin.ogg", Engine::SoundMgr::EFFECT);

		}

		break;

	case ANIM_STATE::ATTACK2:
		if (m_pFrameInfo != nullptr)
		{
			if (m_pFrameInfo->rotFrame % 3 == 0)
			{
				if (mAttackTime >= 0.2f)
				{
					m_pBulletMgr->Add_RecursiveBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_RecursiveBullet", 2, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), 0);
					mAttackTime = 0.f;
				}
				//TY_TPS ZoomIn_180705_ Call ZoomIn Function
				CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
			}
			//DH_SoundSetting_180705_SetSound_SetSound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"bolt_mx_targeting02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"flashmine_ready.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 3, L"g1_shot01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 4, L"g1_shot02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 6, L"g1_shot01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 7, L"g1_shot02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 9, L"g1_shot01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 10, L"g1_shot02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 12, L"g1_shot01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 13, L"g1_shot02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 15, L"g1_shot01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 16, L"g1_shot02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 18, L"g1_shot01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 19, L"g1_shot02.ogg", Engine::SoundMgr::EFFECT);

		}
		break;

	case ANIM_STATE::ATTACK3:
		if (m_bFrameControl == false)
		{
			m_bFrameControl = FrameControl(L"LL_Joint", 10);
		}

		if (m_bFrameControl && mTimeInfo.bLastTime == false)
		{
			LastTime(3.f);
			m_pBossInfo->pTransform->RotationUp(D3DXToRadian(100) * timeDelta);

			if (mAttackTime >= 0.1f)
			{
				//m_pBulletMgr->AddBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 18, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()));
				m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 18, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle() + D3DXToRadian(90.f)), 1);
				m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 18, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle() - D3DXToRadian(180.f)), -1);

				mAttackTime = 0.f;
			}
			else //TY_TPS ZoomIn_180705_ Call ZoomIn Function
			{
				CameraMgr::GetInstance()->ZoomCamera_TPS(*m_pBossInfo->pMyPos);
			}
			//DH_SoundSetting_180705_SetSound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"bolt_mx_targeting01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"robot_butting_act02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 3, L"robot_butting_act01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 7, L"robot_butting_act02.ogg", Engine::SoundMgr::EFFECT);

		}

		break;

	case ANIM_STATE::ATTACK4:
		if (m_bFrameControl == false)
		{
			m_bFrameControl = FrameControl(L"LA_Joint", 10);
		}

		if (m_bFrameControl && mTimeInfo.bLastTime == false)
		{
			LastTime(3.f);
			if (mAttackTime >= 0.1f)
			{
				//m_pSoundMgr->SoundPlay(L"nen_flower_explosion.ogg");
				//TY_AddBullet_180704_ Change Functino Name
				m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 2, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), 1);
				m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 4, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), -1);
				m_pBulletMgr->Add_DirBullet(BulletMgr::BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 7, *m_pBossInfo->pMyPos, *m_pBossInfo->pTransform->GetAngle()), 0);
				mAttackTime = 0.f; mAttackTime = 0.f;
			}

			//DH_SoundSetting_180705_SetSound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"bolt_mx_targeting02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"robot_butting_act01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 5, L"robot_butting_act02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 6, L"robot_butting_act01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 9, L"mnet_ready.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 10, L"nen_flower_explosion.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 16, L"robot_butting_act02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 18, L"robot_butting_act01.ogg", Engine::SoundMgr::EFFECT);

		}

		if (m_pFrameInfo != nullptr)
		{
			if (m_pFrameInfo->rotFrame >= 11)
			{
				m_pSoundMgr->StopSound(Engine::SoundMgr::LOOP_EFFECT);
			}
		}

		break;

	case ANIM_STATE::DESTROY:

		//TY_FixCamera_180704_ Play Action Cam
		if (m_bDead_ActionCam && m_pHpBar->GetDeathCount() != DEATH_COUNT::THREE_BLUE)
		{
			CameraMgr::GetInstance()->InitActionCam(L"path_data_IronmanDead");
			m_bDead_ActionCam = false;
		}

		if (m_pFrameInfo != nullptr)
		{
			//DH_SoundSetting_180705_SetSound
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 1, L"supernova_exp.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 2, L"budd_lightball01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 3, L"mskin.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 4, L"budd_lightball02.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 5, L"budd_lightball01.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 6, L"mnet_ready.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 7, L"mskin.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 9, L"mnet_ready.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 14, L"bomb_03.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 12, L"bursterbeam_fin.ogg", Engine::SoundMgr::EFFECT);
			m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 21, L"d_day_ready.ogg", Engine::SoundMgr::EFFECT);
			if (m_pFrameInfo->rotFrame == 14)
			{
				DestroyPart();
			}
		}

		break;

	case ANIM_STATE::LANDING:
		//DH_SoundSetting_180705_SetSound
		m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 0, L"seismicwave_ready.ogg", Engine::SoundMgr::EFFECT);
		m_pSoundMgr->Set_SoundEffect(m_pFrameInfo, mSoundTime, 7, L"bolt_mx_targeting01.ogg", Engine::SoundMgr::EFFECT);
		break;

	case ANIM_STATE::END:
		break;

	default:
		assert(false && "BossAI::AnimUpdate");
		break;
	}

	//TY_FixCamera_180704_ Set DeadCam Variable True
	if (mAnimState != ANIM_STATE::DESTROY)
	{
		m_bDead_ActionCam = true;
	}
}

void IronAI::SelectAttackMode()
{
	switch (m_pHpBar->GetDeathCount())
	{
	case DEATH_COUNT::ONE_GREEN:
		if (mAttackAnimState == ANIM_STATE::ATTACK0)
		{
			mAnimState = ANIM_STATE::ATTACK4;
			mAIMode = AI_MODE::ATTACK4;
		}
		else
		{
			mAnimState = ANIM_STATE::ATTACK0;
			mAIMode = AI_MODE::ATTACK0;
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
			mAnimState = ANIM_STATE::ATTACK1;
			mAIMode = AI_MODE::ATTACK1;
		}
		else
		{
			mAnimState = ANIM_STATE::ATTACK3;
			mAIMode = AI_MODE::ATTACK3;
		}
		break;
	default:
		assert(false && "IronAI::ChangeAttackAnim");
		break;
	}
}

void IronAI::DestroyPart()
{
	if (*m_pBossInfo->pHP <= 0.f)
	{
		switch (m_pHpBar->GetDeathCount())
		{
		case DEATH_COUNT::ONE_GREEN:
			/*m_pAnimMgr->DeleteAllCube();
			m_pBossObj->SetThanos();
			m_pHpBar->SetDeathCount(DEATH_COUNT::BACK_RED);*/

			EffectMgr::GetInstance()->EffectOperation(L"Explosion_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
				D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(7.f, 7.f, 7.f)));
			
			m_pAnimMgr->Delete_HierachyCube(L"R_Arm");
			break;
		case DEATH_COUNT::TWO_YELLOW:
			EffectMgr::GetInstance()->EffectOperation(L"Explosion_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
				D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(7.f, 7.f, 7.f)));

			m_pAnimMgr->Delete_HierachyCube(L"L_Arm");
			break;
		case DEATH_COUNT::THREE_BLUE:
			EffectMgr::GetInstance()->EffectOperation(L"Explosion_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
				D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(7.f, 7.f, 7.f)));
			EffectMgr::GetInstance()->EffectOperation(L"Explosion2_Effect", EFFECT_INFO(*m_pBossInfo->pMyPos,
				D3DXVECTOR3(0.f, 3.f, 0.f), D3DXVECTOR3(10.f, 10.f, 10.f)));

			m_pAnimMgr->DeleteAllCube();
			m_pBossObj->SetThanos();
			m_pHpBar->SetDeathCount(DEATH_COUNT::BACK_RED);

			m_pPlayerInfo->pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(15.f, 300.f, 2.f));
			PlanetMgr::GetInstance()->Set_CenterPos();
			//TY_ChangeStage_180704_ Change Terrain State
			gTerrainState = TERRAIN_STATE::VIBRATE;
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

IronAI * IronAI::Create(LPDIRECT3DDEVICE9 pDevice, SUPER_CUBE_INFO * pSuperCubeInfo)
{
	IronAI* pInstance = new IronAI(pDevice);

	if (FAILED(pInstance->Init(pSuperCubeInfo)))
	{
		_MSG_BOX("IronAI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::Component * IronAI::Clone()
{
	return new IronAI(*this);
}

DWORD IronAI::Free()
{
	BossAI::Free();
	return 0;
}
