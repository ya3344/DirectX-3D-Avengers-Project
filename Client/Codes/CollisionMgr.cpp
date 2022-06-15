#include "stdafx.h"
#include "CollisionMgr.h"
#include "Sphere.h"
#include "AnimCube.h"
#include "Transform.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "Layer.h"
#include "Player.h"
#include "Boss.h"
#include "BulletMgr.h"
#include "Bullet.h"
#include "ShieldBullet.h"
#include "ParticleCubeMgr.h"

_USING(Client)

_IMPLEMENT_SINGLETON(CollisionMgr)

CollisionMgr::CollisionMgr()
{
}

HRESULT CollisionMgr::Init()
{
	Engine::Layer* pPlayerLayer = Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_STAGE, L"Layer_Player");
	m_pPlayer = (Player*)pPlayerLayer->GetGameObject();

	Engine::Layer* pBossLayer = Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_STAGE, L"Layer_Boss");
	m_pBoss = (Boss*)pBossLayer->GetGameObject();

	m_pBulletList = BulletMgr::GetInstance()->GetBulletList();
	m_pParticleMgr = ParticleCubeMgr::GetInstance();

	return NOERROR;
}

void CollisionMgr::Update()
{
	if (m_pPlayer == nullptr || m_pBoss == nullptr || m_pBulletList == nullptr)
	{
		_MSG_BOX("CollisionMgr Update Failed");
	}

	if (Sphere_Intersection(m_pPlayer->GetSphere(), m_pBoss->GetSphere()))
	{
		if (OBB_Intersection(m_pPlayer->Get_SuperCubeInfo()->pCube, m_pBoss->Get_SuperCubeInfo()->pCube))
		{
#ifdef _DEBUG
#ifdef _COLLISION_DEBUG
			cout << "OBB검사::OBB 충돌!!" << endl;
#endif
#endif
		}
		else
		{
#ifdef _DEBUG
#ifdef _COLLISION_DEBUG
			cout << "OBB검사::OBB 충돌 안함!!" << endl;
#endif
#endif
		}
	}
	else
	{
#ifdef _DEBUG
#ifdef _COLLISION_DEBUG
		cout << "구충돌검사::구 충돌 안함!!" << endl;
#endif
#endif
	}

	//TY_FixBullet_180630 Fix Bullet Data Type
	for (Engine::GameObject* pBullet : m_pBulletList[BulletMgr::PLAYER])
	{
		Bullet* pBl = (Bullet*)(pBullet);
		if (Sphere_Intersection(pBl->GetSphere(), m_pBoss->GetSphere()))
		{
			//TY_ChangeStage_180705_ Change Value
			D3DXVECTOR3 vParticlePos = *m_pBoss->Get_SuperCubeInfo()->pMyPos;
			D3DXVECTOR3 vParticleDir = -*pBl->GetDir();


			pBl->SetDead();

			bool bDamage = m_pBoss->SetDamage(0.2f);

			if (bDamage)
			{
				m_pParticleMgr->Add_PartiCleCube(PARTICLE_INFO(vParticlePos, vParticleDir, DEATH_COUNT::BACK_RED, 2, 0.1f));
				m_pParticleMgr->Add_PartiCleCube(PARTICLE_INFO(vParticlePos, vParticleDir, DEATH_COUNT::TWO_YELLOW, 2, 0.1f));

			}
		}
	}

	for (Engine::GameObject* pBullet : m_pBulletList[BulletMgr::BOSS])
	{
		Bullet* pBl = (Bullet*)(pBullet);
		if (Sphere_Intersection(pBl->GetSphere(), m_pPlayer->GetSphere()))
		{
			//TY_ChangeStage_180705_ Change Value
			D3DXVECTOR3 vParticlePos = *m_pPlayer->Get_SuperCubeInfo()->pMyPos;
			D3DXVECTOR3 vParticleDir = -*pBl->GetDir();

			pBl->SetDead();
			m_pPlayer->SetDamage(0.02f);

			m_pParticleMgr->Add_PartiCleCube(PARTICLE_INFO(vParticlePos, vParticleDir, DEATH_COUNT::BACK_RED, 5, 0.1f));
			//DH_SoundMgr_180706 Setting Sound
			Engine::SoundMgr::GetInstance()->SoundPlay(L"gun_hit.ogg ", Engine::SoundMgr::BOSS);

		}
	}

	//TY_FixBullet_180701_Add Function To Get Shield
	ShieldBullet* pSheield = m_pPlayer->GetShield();
	if (pSheield->IsActive() && Sphere_Intersection(pSheield->GetSphere(), m_pBoss->GetSphere()))
	{
		//TY_ChangeStage_180705_ Change Value
		D3DXVECTOR3 vParticlePos = *m_pBoss->Get_SuperCubeInfo()->pMyPos;
		D3DXVECTOR3 vParticleDir = -*pSheield->GetDir();

		m_pParticleMgr->Add_PartiCleCube(PARTICLE_INFO(vParticlePos, vParticleDir, DEATH_COUNT::BACK_RED, 2, 0.2f));
		m_pParticleMgr->Add_PartiCleCube(PARTICLE_INFO(vParticlePos, vParticleDir, DEATH_COUNT::TWO_YELLOW, 2, 0.2f));
		m_pBoss->SetDamage(1.f);

		pSheield->SetActive(false);
		pSheield->SetEquip(true);

		// JH_ShieldRender_180703 ShiledRender true
		m_pPlayer->SetShield_RenderState(false);

		//DH_SoundMgr_180706 Setting Sound
		Engine::SoundMgr::GetInstance()->SoundPlay(L"tigerstrike_01.ogg", Engine::SoundMgr::BOSS);

	}

	if (!pSheield->IsEquip() && Sphere_Intersection(pSheield->GetSphere(), m_pPlayer->GetSphere()))
	{
		pSheield->SetEquip(true);
		pSheield->SetActive(false);

		// JH_ShieldRender_180703 ShiledRender true
		m_pPlayer->SetShield_RenderState(false);
	}
}

int CollisionMgr::OBB_Intersection(AnimCube * pCube1, AnimCube * pCube2)
{
	if (nullptr == pCube1
		|| nullptr == pCube2)
	{
		_MSG_BOX("BoxIntersect Failed");
		return 0;
	}

	Engine::Transform* pTrans1 = pCube1->GetTransform();
	Engine::Transform* pTrans2 = pCube2->GetTransform();


	// Get Length Between Two Boxes
	D3DXVECTOR3 vInterLen = *pTrans1->GetInfo(Engine::Transform::INFO_POSITION) - *pTrans2->GetInfo(Engine::Transform::INFO_POSITION);

	D3DXVECTOR3 vAxisDot[3];
	D3DXVECTOR3 vAbsAxisDot[3];
	D3DXVECTOR3 vLenDot;
	float R0, R1, R;    // Length Projected To Separating Line By OBB
	float R01;        //=R0+R1

	D3DXVECTOR3 vExtent[2];
	vExtent[0] = { pTrans1->GetScale().x*0.5f, 1.f , pTrans1->GetScale().z*0.5f };
	vExtent[1] = { pTrans2->GetScale().x*0.5f, 1.f  , pTrans2->GetScale().z*0.5f };

	D3DXVECTOR3 vRight[2], vUp[2], vLook[2];
	vRight[0] = pTrans1->GetWolrd().m[0];
	vRight[1] = pTrans2->GetWolrd().m[0];
	vUp[0] = pTrans1->GetWolrd().m[1];
	vUp[1] = pTrans2->GetWolrd().m[1];
	vLook[0] = pTrans1->GetWolrd().m[2];
	vLook[1] = pTrans2->GetWolrd().m[2];

	//////////////////////////////////////////////// Start Comparison

	//A0
	vAxisDot[0][0] = D3DXVec3Dot(&vRight[0], &vRight[1]);
	vAxisDot[0][1] = D3DXVec3Dot(&vRight[0], &vUp[1]);
	vAxisDot[0][2] = D3DXVec3Dot(&vRight[0], &vLook[1]);
	vLenDot[0] = D3DXVec3Dot(&vRight[0], &vInterLen);
	vAbsAxisDot[0][0] = (float)fabsf(vAxisDot[0][0]);
	vAbsAxisDot[0][1] = (float)fabsf(vAxisDot[0][1]);
	vAbsAxisDot[0][2] = (float)fabsf(vAxisDot[0][2]);
	R = (float)fabsf(vLenDot[0]);
	R1 = vExtent[1][1] * vAbsAxisDot[0][0] + vExtent[1][1] * vAbsAxisDot[0][1] + vExtent[1][1] * vAbsAxisDot[0][2];
	R01 = vExtent[0][0] + R1;
	if (R>R01)return 0;

	//A1
	vAxisDot[1][0] = D3DXVec3Dot(&vUp[0], &vRight[1]);
	vAxisDot[1][1] = D3DXVec3Dot(&vUp[0], &vUp[1]);
	vAxisDot[1][2] = D3DXVec3Dot(&vUp[0], &vLook[1]);
	vLenDot[1] = D3DXVec3Dot(&vUp[0], &vInterLen);
	vAbsAxisDot[1][0] = (float)fabsf(vAxisDot[1][0]);
	vAbsAxisDot[1][1] = (float)fabsf(vAxisDot[1][1]);
	vAbsAxisDot[1][2] = (float)fabsf(vAxisDot[1][2]);
	R = (float)fabsf(vLenDot[1]);
	R1 = vExtent[1][0] * vAbsAxisDot[1][0] + vExtent[1][1] * vAbsAxisDot[1][1] + vExtent[1][2] * vAbsAxisDot[1][2];
	R01 = vExtent[0][1] + R1;
	if (R>R01)return 0;

	//A2
	vAxisDot[2][0] = D3DXVec3Dot(&vLook[0], &vRight[1]);
	vAxisDot[2][1] = D3DXVec3Dot(&vLook[0], &vUp[1]);
	vAxisDot[2][2] = D3DXVec3Dot(&vLook[0], &vLook[1]);
	vLenDot[2] = D3DXVec3Dot(&vLook[0], &vInterLen);
	vAbsAxisDot[2][0] = (float)fabsf(vAxisDot[2][0]);
	vAbsAxisDot[2][1] = (float)fabsf(vAxisDot[2][1]);
	vAbsAxisDot[2][2] = (float)fabsf(vAxisDot[2][2]);
	R = (float)fabsf(vLenDot[2]);
	R1 = vExtent[1][0] * vAbsAxisDot[2][0] + vExtent[1][1] * vAbsAxisDot[2][1] + vExtent[1][2] * vAbsAxisDot[2][2];
	R01 = vExtent[0][2] + R1;
	if (R>R01)return 0;

	//B0
	R = (float)fabsf(D3DXVec3Dot(&vRight[1], &vInterLen));
	R0 = vExtent[0][0] * vAbsAxisDot[0][0] + vExtent[0][1] * vAbsAxisDot[1][0] + vExtent[0][2] * vAbsAxisDot[2][0];
	R01 = R0 + vExtent[1][0];
	if (R>R01)return 0;

	//B1
	R = (float)fabsf(D3DXVec3Dot(&vUp[1], &vInterLen));
	R0 = vExtent[0][0] * vAbsAxisDot[0][1] + vExtent[0][1] * vAbsAxisDot[1][1] + vExtent[0][2] * vAbsAxisDot[2][1];
	R01 = R0 + vExtent[1][1];
	if (R>R01)return 0;

	//B2
	R = (float)fabsf(D3DXVec3Dot(&vLook[1], &vInterLen));
	R0 = vExtent[0][0] * vAbsAxisDot[0][2] + vExtent[0][1] * vAbsAxisDot[1][2] + vExtent[0][2] * vAbsAxisDot[2][2];
	R01 = R0 + vExtent[1][2];
	if (R>R01)return 0;

	//A0xB0
	R = (float)fabsf(vLenDot[2] * vAxisDot[1][0] - vLenDot[1] * vAxisDot[2][0]);
	R0 = vExtent[0][1] * vAbsAxisDot[2][0] + vExtent[0][2] * vAbsAxisDot[1][0];
	R1 = vExtent[1][1] * vAbsAxisDot[0][2] + vExtent[1][2] * vAbsAxisDot[0][1];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A0xB1
	R = (float)fabsf(vLenDot[2] * vAxisDot[1][1] - vLenDot[1] * vAxisDot[2][1]);
	R0 = vExtent[0][1] * vAbsAxisDot[2][1] + vExtent[0][2] * vAbsAxisDot[1][1];
	R1 = vExtent[1][0] * vAbsAxisDot[0][2] + vExtent[1][2] * vAbsAxisDot[0][0];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A0xB2
	R = (float)fabsf(vLenDot[2] * vAxisDot[1][2] - vLenDot[1] * vAxisDot[2][2]);
	R0 = vExtent[0][1] * vAbsAxisDot[2][2] + vExtent[0][2] * vAbsAxisDot[1][2];
	R1 = vExtent[1][0] * vAbsAxisDot[0][1] + vExtent[1][1] * vAbsAxisDot[0][0];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A1xB0
	R = (float)fabsf(vLenDot[0] * vAxisDot[2][0] - vLenDot[2] * vAxisDot[0][0]);
	R0 = vExtent[0][0] * vAbsAxisDot[2][0] + vExtent[0][2] * vAbsAxisDot[0][0];
	R1 = vExtent[1][1] * vAbsAxisDot[1][2] + vExtent[1][2] * vAbsAxisDot[1][1];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A1xB1
	R = (float)fabsf(vLenDot[0] * vAxisDot[2][1] - vLenDot[2] * vAxisDot[0][1]);
	R0 = vExtent[0][0] * vAbsAxisDot[2][1] + vExtent[0][2] * vAbsAxisDot[0][1];
	R1 = vExtent[1][0] * vAbsAxisDot[1][2] + vExtent[1][2] * vAbsAxisDot[1][0];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A1xB2
	R = (float)fabsf(vLenDot[0] * vAxisDot[2][2] - vLenDot[2] * vAxisDot[0][2]);
	R0 = vExtent[0][0] * vAbsAxisDot[2][2] + vExtent[0][2] * vAbsAxisDot[0][2];
	R1 = vExtent[1][0] * vAbsAxisDot[1][1] + vExtent[1][1] * vAbsAxisDot[1][0];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A2xB0
	R = (float)fabsf(vLenDot[1] * vAxisDot[0][0] - vLenDot[0] * vAxisDot[1][0]);
	R0 = vExtent[0][0] * vAbsAxisDot[1][0] + vExtent[0][1] * vAbsAxisDot[0][0];
	R1 = vExtent[1][1] * vAbsAxisDot[2][2] + vExtent[1][2] * vAbsAxisDot[2][1];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A2xB1
	R = (float)fabsf(vLenDot[1] * vAxisDot[0][1] - vLenDot[0] * vAxisDot[1][1]);
	R0 = vExtent[0][0] * vAbsAxisDot[1][1] + vExtent[0][1] * vAbsAxisDot[0][1];
	R1 = vExtent[1][0] * vAbsAxisDot[2][2] + vExtent[1][2] * vAbsAxisDot[2][0];
	R01 = R0 + R1;
	if (R>R01)return 0;

	//A2xB2
	R = (float)fabsf(vLenDot[1] * vAxisDot[0][2] - vLenDot[0] * vAxisDot[1][2]);
	R0 = vExtent[0][0] * vAbsAxisDot[1][2] + vExtent[0][1] * vAbsAxisDot[0][2];
	R1 = vExtent[1][0] * vAbsAxisDot[2][1] + vExtent[1][1] * vAbsAxisDot[2][0];
	R01 = R0 + R1;
	if (R>R01)return 0;

	return 1;
}

int CollisionMgr::Sphere_Intersection(const Sphere * pSphere1, const Sphere * pSphere2)
{
	if (nullptr == pSphere1
		|| nullptr == pSphere2)
	{
		return 0;
	}

	D3DXVECTOR3 vInter = pSphere1->GetCenterPos() - pSphere2->GetCenterPos();

	float interLength = D3DXVec3Length(&vInter);
	float radiusSum = pSphere1->GetRadius() + pSphere2->GetRadius();

	//cout << "사이 길이 : "<< interLength <<endl;
	//cout << "반지름 함: " << radiusSum << endl;

	return interLength <= radiusSum;
}

DWORD CollisionMgr::Free()
{
	return 0;
}