#include "stdafx.h"
#include "BulletMgr.h"
#include "Layer.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "ParticleCubeMgr.h"
#include "CameraMgr.h"

#include "Bullet.h"
#include "NormalBullet.h"
#include "ShieldBullet.h"
#include "WallBullet.h"
#include "RecursiveBullet.h"
#include "DNABullet.h"
#include "MeteorBullet.h"
#include "SearchBullet.h"
#include "CircleBullet.h"
#include "DigdaBullet.h"

_USING(Client)

_IMPLEMENT_SINGLETON(BulletMgr)

BulletMgr::BulletMgr()
{
}

UPDATE BulletMgr::Update(const float timeDelta)
{
	KeyCheck();

	for (size_t i = 0; i < OBJ_END; i++)
	{
		if (m_pBulletSpace[i].size() <= 0)
			continue;

		for (Engine::GameObject* pBulletObj : m_pBulletSpace[i])
		{
			if (pBulletObj == nullptr)
				return UPDATE::FAIL;

			pBulletObj->Update(timeDelta);
		}
	}
	return UPDATE::NORMAL;
}

UPDATE BulletMgr::LastUpdate(const float timeDelta)
{
	mTimeDelta += timeDelta;

	for (size_t i = 0; i < OBJ_END; i++)
	{
		if (m_pBulletSpace[i].size() <= 0)
			continue;

		auto iterBegin = m_pBulletSpace[i].begin();
		auto iterEnd = m_pBulletSpace[i].end();

		Bullet* pBullet = nullptr;

		for (; iterBegin != iterEnd; )
		{
			pBullet = (Bullet*)(*iterBegin);
			mUpdate = pBullet->LastUpdate(timeDelta);

			//TY_AddBullet_180704 Change if To Switch
			D3DXVECTOR3 bulletPos = { 0.f, 0.f, 0.f };
			float bulletAngle = 0.f;
			int recursiveCount = 0;

			switch (mUpdate)
			{
			case UPDATE::DIE:
				Engine::Safe_Release(pBullet);
				iterBegin = m_pBulletSpace[i].erase(iterBegin);

				break;
			case UPDATE::BULLET_WALL:
				bulletPos = *pBullet->GetTransform()->GetInfo(Engine::Transform::INFO_POSITION);
				bulletAngle = *pBullet->GetTransform()->GetAngle();
				for (int i = 0; i < 3; ++i)
				{
					AddBullet(BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_WallBullet", 17, bulletPos, bulletAngle + 115.f + 65.f*i));
				}
				Engine::Safe_Release(pBullet);
				iterBegin = m_pBulletSpace[i].erase(iterBegin);

				break;
			case UPDATE::BULLET_BOMB:
				bulletPos = *pBullet->GetTransform()->GetInfo(Engine::Transform::INFO_POSITION);
				recursiveCount = dynamic_cast<RecursiveBullet*>(pBullet)->PlusRecursiveCount();
				for (int i = 0; i < 5; ++i)
				{
					Add_RecursiveBullet(BOSS, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_RecursiveBullet", recursiveCount + 2, bulletPos, 72.f*i), recursiveCount);
				}
				Engine::Safe_Release(pBullet);
				iterBegin = m_pBulletSpace[i].erase(iterBegin);

				break;
			case UPDATE::BULLET_METEOR:
				Engine::Safe_Release(pBullet);
				iterBegin = m_pBulletSpace[i].erase(iterBegin);

				break;
			default:
				++iterBegin;
				break;
			}

			//			if (mUpdate == UPDATE::DIE || mUpdate == UPDATE::BULLET_WALL || mUpdate == UPDATE::BULLET_BOMB)
			//			{
			//#ifdef _DEBUG
			//#ifdef _BULLET_DEBUG
			//				cout << "ÃÑ¾Ë°¹¼ö :" << m_pBulletSpace[i].size() << endl;
			//#endif
			//#endif
			//			}
			//			else
			//			{
			//				++iterBegin;
			//			}
		}
	}

	return UPDATE::NORMAL;
}

HRESULT BulletMgr::AddBullet(BULLET_OBJ obj, const BULLET_INFO& bulletInfo)
{
	TCHAR namePath[MAX_PATH] = L"";

	wsprintf(namePath, L"spherePath%d", mCount);
	mTagSpace.emplace_back(namePath);

	Bullet* pPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(bulletInfo.sceneIdx, bulletInfo.pProtoTag);
	if (pPrototype == nullptr)
		return E_FAIL;

	m_pBulletSpace[obj].emplace_back(pPrototype->Clone(bulletInfo, mTagSpace[mCount].c_str()));

	++mCount;

	return NOERROR;
}

HRESULT BulletMgr::Add_RecursiveBullet(BULLET_OBJ obj, const BULLET_INFO & bulletInfo, const int recursiveCount)
{
	TCHAR namePath[MAX_PATH] = L"";

	wsprintf(namePath, L"spherePath%d", mCount);
	mTagSpace.emplace_back(namePath);

	Bullet* pPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, L"Prototype_GameObject_RecursiveBullet");
	if (pPrototype == nullptr)
		return E_FAIL;

	m_pBulletSpace[obj].emplace_back(pPrototype->Clone(bulletInfo, mTagSpace[mCount].c_str(), recursiveCount));

	++mCount;

	return NOERROR;
}

HRESULT BulletMgr::Add_DigdaBullet(BULLET_OBJ obj, const BULLET_INFO & bulletInfo, const SUPER_CUBE_INFO * pTargetInfo, const int dir)
{
	TCHAR namePath[MAX_PATH] = L"";

	wsprintf(namePath, L"spherePath%d", mCount);
	mTagSpace.emplace_back(namePath);

	Bullet* pPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, L"Prototype_GameObject_DigdaBullet");
	if (pPrototype == nullptr)
		return E_FAIL;

	//const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo, const TCHAR* pSphereTag, const int dir
	m_pBulletSpace[obj].emplace_back(pPrototype->Clone(bulletInfo, pTargetInfo, mTagSpace[mCount].c_str(), dir));

	++mCount;

	return NOERROR;
}

HRESULT BulletMgr::Add_DirBullet(BULLET_OBJ obj, const BULLET_INFO & bulletInfo, const int dir)
{
	TCHAR namePath[MAX_PATH] = L"";

	wsprintf(namePath, L"spherePath%d", mCount);
	mTagSpace.emplace_back(namePath);

	Bullet* pPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, bulletInfo.pProtoTag);
	if (pPrototype == nullptr)
		return E_FAIL;

	m_pBulletSpace[obj].emplace_back(pPrototype->Clone(bulletInfo, mTagSpace[mCount].c_str(), dir));

	++mCount;

	return NOERROR;
}

//TY_FixChangeTerrain_180704_ Function To Add Search Bullet
HRESULT BulletMgr::Add_SearchBullet(BULLET_OBJ obj, const BULLET_INFO & bulletInfo, const SUPER_CUBE_INFO * pTargetInfo)
{
	TCHAR namePath[MAX_PATH] = L"";

	wsprintf(namePath, L"spherePath%d", mCount);
	mTagSpace.emplace_back(namePath);

	Bullet* pPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, L"Prototype_GameObject_SearchBullet");
	if (pPrototype == nullptr)
		return E_FAIL;

	m_pBulletSpace[obj].emplace_back(pPrototype->Clone(bulletInfo, pTargetInfo, mTagSpace[mCount].c_str()));

	++mCount;

	return NOERROR;
}

HRESULT BulletMgr::Ready_BulletPrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;

	pGameObject = NormalBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_NormalBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	pGameObject = ShieldBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_ShieldBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	pGameObject = WallBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_WallBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	pGameObject = RecursiveBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_RecursiveBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	pGameObject = DNABullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_DNABullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	//TY_AddBullet_180704_ Add Meteor Bullet Prototype
	pGameObject = MeteorBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_MeteorBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	//TY_AddBullet_180704_ Add Search Bullet Prototype
	pGameObject = SearchBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_SearchBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	//TY_AddBullet_180704_ Add Circle Bullet Prototype
	pGameObject = CircleBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_CircleBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	//TY_AddBullet_180704_ Add Digda Bullet Prototype
	pGameObject = DigdaBullet::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_DigdaBullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

void BulletMgr::KeyCheck()
{
	//if (Engine::KeyMgr::GetInstance()->KeyPressing(DIK_Z) && mTimeDelta >= 0.1f)
	//{
	//	Add_SearchBullet(PLAYER, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_SearchBullet", 8, *m_pPlayerInfo->pMyPos, *m_pPlayerInfo->pTransform->GetAngle()), m_pBossInfo);

	//	mTimeDelta = 0.f;
	//}
	//else if (Engine::KeyMgr::GetInstance()->KeyPressing(DIK_X) && mTimeDelta >= 0.1f)
	//{
	//	for (int i = 0; i < 8; ++i)
	//	{
	//		Add_DirBullet(PLAYER, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_CircleBullet", 0, *m_pPlayerInfo->pMyPos, *m_pPlayerInfo->pTransform->GetAngle()), i);
	//	}
	//	mTimeDelta = 0.f;
	//}
	//else if (Engine::KeyMgr::GetInstance()->KeyPressing(DIK_C) && mTimeDelta >= 0.1f)
	//{
	//	Add_DirBullet(PLAYER, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 2, *m_pPlayerInfo->pMyPos, *m_pPlayerInfo->pTransform->GetAngle()), 1);
	//	Add_DirBullet(PLAYER, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 3, *m_pPlayerInfo->pMyPos, *m_pPlayerInfo->pTransform->GetAngle()), -1);
	//	Add_DirBullet(PLAYER, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DNABullet", 1, *m_pPlayerInfo->pMyPos, *m_pPlayerInfo->pTransform->GetAngle()), 0);
	//	mTimeDelta = 0.f;
	//}
	//else if (Engine::KeyMgr::GetInstance()->KeyPressing(DIK_V) && mTimeDelta >= 0.1f)
	//{
	//	for (int i = 0; i < 8; ++i)
	//	{
	//		Add_DigdaBullet(PLAYER, BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_DigdaBullet", 11, *m_pPlayerInfo->pMyPos, *m_pPlayerInfo->pTransform->GetAngle()), m_pBossInfo, i);
	//	}

	//	mTimeDelta = 0.f;
	//}
}


DWORD BulletMgr::Free()
{
	for (size_t i = 0; i < OBJ_END; i++)
	{
		for (Engine::GameObject* pBullet : m_pBulletSpace[i])
		{
			Engine::Safe_Release(pBullet);
		}
	}

	return DWORD();
}
