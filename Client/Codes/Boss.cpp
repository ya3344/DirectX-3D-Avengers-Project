#include "stdafx.h"
#include "Boss.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "AnimCube.h"
#include "Layer.h"
#include "Player.h"
#include "IronAI.h"
#include "ThanosAI.h"
#include "Sphere.h"
#include "DetectRect.h"
#include "HpBar.h"

_USING(Client)

Boss::Boss(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
	, m_pEffectMgr(EffectMgr::GetInstance())
{
	m_pEffectMgr->AddRef();
}

Boss::Boss(const Boss & rhs)
	: Engine::GameObject(rhs)
	, mSuperCube_Info(rhs.mSuperCube_Info)
	, m_pEffectMgr(rhs.m_pEffectMgr)
	
{
	m_pAnimCube[IRON] = rhs.m_pAnimCube[IRON];
	m_pAnimCube[THANOS] = rhs.m_pAnimCube[THANOS];
	m_pEffectMgr->AddRef();
}


HRESULT Boss::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pAnimMgr->Set_AnimLayer(Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_BOSS, L"Layer_Boss_AnimCube"));

	if (FAILED(m_pAnimMgr->SetHierachy()))
	{
		_MSG_BOX("Boss Hierachy Failed!");
	}

	for (size_t i = 0; i < END; i++)
	{
		if (FAILED(m_pBossAI[i]->SetBoss(this)))
		{
			_MSG_BOX("BossAI SetBoss Failed!");
		}

		// BossAI PlayerInfo Setting
		if (FAILED(m_pBossAI[i]->SetPlayer()))
		{
			_MSG_BOX("BossAI SetPlayer Failed!");
		}

		//JH_AnimFrame_180624 AnimMgr Setting
		if (FAILED(m_pBossAI[i]->SetAnimMgr(m_pAnimMgr)))
		{
			_MSG_BOX("BossAI SetAnimMgr Failed!");
		}

		//JH_AnimFrame_180624 DetectRect Setting
		if (FAILED(m_pBossAI[i]->SetDetectRect(m_pDetectRect)))
		{
			_MSG_BOX("BossAI SetDetectRect Failed!");
		}

		// JH_NaturalAnim_1800630 HpBar Setting
		if (FAILED(m_pBossAI[i]->SetHpBar(m_pHpBar)))
		{
			_MSG_BOX("BossAI SetHpBar Failed!");
		}

	}
	
	m_pCurrentAI = m_pBossAI[IRON];
	// DH_BulletAdd_180618  BpssInfo Setting
	BulletMgr::GetInstance()->SetBossInfo(&mSuperCube_Info);
	m_pHpBar->SetHp(*mSuperCube_Info.pHP);
	
	return NOERROR;
}

UPDATE Boss::Update(const float timeDelta)
{
	// JH_AnimBugFix_180702 FreeConrol Fix
	if (m_bFreeControl)
	{
		KeyCheck(timeDelta);
	}
	else
	{
		m_pCurrentAI->AI_Mode(timeDelta);
	}

	m_pCurrentAI->AnimState_Change(timeDelta);
	m_pCurrentAI->AnimUpdate(timeDelta);

	m_pAnimMgr->InterpolationUpdate(timeDelta);

	if (mSuperCube_Info.pAnimNode != nullptr)
	{
		mSuperCube_Info.pAnimNode->RecursiveMatrix();
	}

	// JH_AnimBugFix_180702 FreeConrol Fix
	//if (m_pKeyMgr->KeyDown(DIK_9))
	//{
	//	SetThanos();
	//}

	if (m_pKeyMgr->KeyDown(DIK_0))
	{
		m_bFreeControl = !m_bFreeControl;
		m_pCurrentAI->Set_AnimState(ANIM_STATE::WALK);
		m_pCurrentAI->Set_AIMode(AI_MODE::WALK);
	}

	if (m_pKeyMgr->KeyPressing(DIK_F10) && gTerrainState <= TERRAIN_STATE::STAGE2_PLAY)
	{
		SetDebugThanos();
	}
	


	return Engine::GameObject::Update(timeDelta);

}

UPDATE Boss::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void Boss::Render()
{
	////TY_HP_180626 Render HpBar
	//if (gTerrainState != TERRAIN_STATE::STAGE1_PLAY && gTerrainState != TERRAIN_STATE::STAGE2_PLAY)
	//	return;

	m_pAnimMgr->Render();
	m_pSphere->Render();


	m_pHpBar->Render();
}

HRESULT Boss::ReadyPrototype()
{
	if (FAILED(Ready_LayerAnimCube()))
	{
		_MSG_BOX("Boss FileLoad Failed!");
		return E_FAIL;
	}

	return NOERROR;
}

void Boss::SetThanos()
{
	m_pAnimMgr->Set_AnimLayer(Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_THANOS, L"Layer_Thanos_AnimCube"));
	if (FAILED(m_pAnimMgr->SetHierachy()))
	{
		_MSG_BOX("Boss Hierachy Failed!");
	}

	Set_SuperCube(m_pAnimCube[THANOS], 8.f, D3DXVECTOR3(15.f, 300.f, 18.f));
	//Set_SuperCube(m_pAnimCube[THANOS], 12.f, D3DXVECTOR3(15.f, 3.5f, 18.f));
	m_pCurrentAI = m_pBossAI[THANOS];
	m_pCurrentAI->SetBossInfo(&mSuperCube_Info);

	m_pSphere->SetGameObject(mSuperCube_Info.pTransform);
	m_pHpBar->SetGameObject(mSuperCube_Info.pTransform);

	m_pCurrentAI->Set_AnimState(ANIM_STATE::LANDING);
	m_pCurrentAI->Set_AIMode(AI_MODE::LANDING);
	m_pHpBar->SetHp(*mSuperCube_Info.pHP);
}

void Boss::SetDebugThanos()
{
	static bool bInit = false;

	if (bInit == false)
	{
		Set_SuperCube(m_pAnimCube[THANOS], 8.f, D3DXVECTOR3(15.f, -26.5f, 18.f));
		m_pCurrentAI = m_pBossAI[THANOS];
		m_pCurrentAI->SetBossInfo(&mSuperCube_Info);

		m_pSphere->SetGameObject(mSuperCube_Info.pTransform);
		m_pHpBar->SetGameObject(mSuperCube_Info.pTransform);

		m_pCurrentAI->Set_AnimState(ANIM_STATE::LANDING);
		m_pCurrentAI->Set_AIMode(AI_MODE::LANDING);
		m_pHpBar->SetHp(*mSuperCube_Info.pHP);
		bInit = true;
	}
	
}

HRESULT Boss::Ready_LayerAnimCube()
{
	Engine::Component* pComponent = nullptr;

	{
		// 슈퍼부모 속성 받아오기
		m_pAnimCube[THANOS] = (AnimCube*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_THANOS, L"Layer_Thanos_AnimCube", L"Waist");

		// 슈퍼부모 속성 적용
		Set_SuperCube(m_pAnimCube[THANOS], 12.f, D3DXVECTOR3(15.f, 300.f, 18.f));
		pComponent = ThanosAI::Create(m_pDevice, &mSuperCube_Info);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STAGE, L"Component_ThanosAI", pComponent)))
			return E_FAIL;
	}

	{
		m_pAnimCube[IRON] = (AnimCube*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_BOSS, L"Layer_Boss_AnimCube", L"Waist");

		// 슈퍼부모 속성 적용
		//TY_ChangeStage_180704_ Set Boss Position
#ifdef _TERRAIN_INIT_DEBUG
		Set_SuperCube(m_pAnimCube[IRON], 6.f, D3DXVECTOR3(15.f, 3.5f, 18.f));
#else
		Set_SuperCube(m_pAnimCube[IRON], 6.f, D3DXVECTOR3(15.f, 100.f, 18.f));
#endif
		// JH_AnimFrameConfrim_180702 For.Component_IronAI, ThanosAI
		pComponent = IronAI::Create(m_pDevice, &mSuperCube_Info);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STAGE, L"Component_BossAI", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT Boss::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Com_Renderer
		pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
			return E_FAIL;
	}

	{
		// For.Com_AnimMgr
		pComponent = m_pAnimMgr = (AnimMgr*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_AnimMgr");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_AnimMgr", pComponent)))
			return E_FAIL;
	}

	// For.Com_BossAI
	{
		pComponent = m_pBossAI[IRON] = (BossAI*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STAGE, L"Component_BossAI");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_BossAI", pComponent)))
			return E_FAIL;
	}

	// For.Com_ThanosAI
	{
		pComponent = m_pBossAI[THANOS] = (BossAI*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STAGE, L"Component_ThanosAI");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_ThanosAI", pComponent)))
			return E_FAIL;
	}

	//TY_Sphere_180618 Add Sphere Object
	{
		Engine::GameObject* pGameObject = m_pSphere = Sphere::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		m_pSphere->AddRef();

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Boss_Sphere", SCENE_STAGE, m_pSphere)))
		{
			_MSG_BOX("Add Sphere Failed");
		}

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_Boss_Sphere", SCENE_STAGE, L"Layer_Sphere")))
		{
			_MSG_BOX("Add Sphere Failed");
		}

		m_pSphere->SetGameObject(mSuperCube_Info.pTransform);
		m_pSphere->SetRadius(2.f);

		//TY_DectRect_180618 Add DetectRect Prototype
		pGameObject = m_pDetectRect = DetectRect::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_DetectRect", SCENE_STAGE, pGameObject)))
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_DetectRect", SCENE_STAGE, L"Layer_DetectRect")))
			return E_FAIL;

		m_pDetectRect->SetTarget((Player*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Player"));
		m_pDetectRect->SetBossInfo(&mSuperCube_Info);

		//TY_HP_180626 Add HpBar
		pGameObject = m_pHpBar = HpBar::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		m_pHpBar->AddRef();

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Boss_HpBar", SCENE_STAGE, m_pHpBar)))
		{
			_MSG_BOX("Add HpBar Failed");
		}

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_Boss_HpBar", SCENE_STAGE, L"Layer_HpBar")))
		{
			_MSG_BOX("Add HpBar Failed");
		}

		m_pHpBar->SetGameObject(mSuperCube_Info.pTransform);
		m_pHpBar->SetSize(D3DXVECTOR3{ 6.f, 0.5f, 0.5f });
		m_pHpBar->SetHeight(4.f);
	}

	return NOERROR;
}

HRESULT Boss::KeyCheck(const float timeDelta)
{
	if (m_pCurrentAI->Get_AnimState() == ANIM_STATE::WALK)
	{
		m_pCurrentAI->Set_AnimState(ANIM_STATE::STAND);
	}

	if (m_pKeyMgr->KeyPressing(DIK_I))
	{
		mSuperCube_Info.pTransform->GoStraight(5.0f * timeDelta);
		m_pCurrentAI->Set_AnimState(ANIM_STATE::WALK);
	}

	if (m_pKeyMgr->KeyPressing(DIK_K))
	{
		mSuperCube_Info.pTransform->GoStraight(-5.0f * timeDelta);
		m_pCurrentAI->Set_AnimState(ANIM_STATE::WALK);
	}

	if (m_pKeyMgr->KeyPressing(DIK_J))
	{
		//-90.f
		mSuperCube_Info.pTransform->RotationUp(D3DXToRadian(-90.f) * timeDelta);
	}

	if (m_pKeyMgr->KeyPressing(DIK_L))
	{
		//90
		mSuperCube_Info.pTransform->RotationUp(D3DXToRadian(90.f) * timeDelta);
	}

	if (m_pKeyMgr->KeyPressing(DIK_P))
	{
		if (*mSuperCube_Info.pHP >= 0)
		{
			*mSuperCube_Info.pHP -= 0.5f;
		}

		if (*mSuperCube_Info.pHP <= 0)
		{
			*mSuperCube_Info.pHP = 0;
		}

		m_pHpBar->SetHp(*mSuperCube_Info.pHP);
	}


	return NOERROR;
}

void Boss::Set_SuperCube(AnimCube* pAnimCube, const float HP, const D3DXVECTOR3 vPos)
{
	if (pAnimCube == nullptr)
	{
		_MSG_BOX("Boss Set SuperCube Failed!");
		return;
	}

	mSuperCube_Info.pCube = pAnimCube;

	mSuperCube_Info.pAnimNode = mSuperCube_Info.pCube->Get_AnimNode();

	mSuperCube_Info.pTransform = mSuperCube_Info.pCube->GetTransform();

	mSuperCube_Info.pMyPos = mSuperCube_Info.pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	mSuperCube_Info.pMyLook = mSuperCube_Info.pTransform->GetInfo(Engine::Transform::INFO_LOOK);

	// Initialize Intro_Debug
	mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &vPos);
	mSuperCube_Info.pTransform->SetRotation(D3DXToRadian(180.f));

	//TY_HP_180628 Initialize Hp
	mSuperCube_Info.pHP = &mHP;
	mSuperCube_Info.pMaxHP = &mMaxHP;
	*mSuperCube_Info.pHP = HP;
	*mSuperCube_Info.pMaxHP = HP;
}

// JH_NaturalAnim_1800630 HpBar Setting SetDamage Confirm
//TY_ChangeStage_180705_Add Return Type Bool
bool Boss::SetDamage(const float hp)
{
	if (m_pCurrentAI->Get_AnimState() == ANIM_STATE::DESTROY || m_pCurrentAI->Get_AnimState() == ANIM_STATE::LANDING)
		return false;

	//TY_HP_180628 Set Hp 
	if (*mSuperCube_Info.pHP >= 0)
	{
		*mSuperCube_Info.pHP -= hp;
		m_pEffectMgr->EffectOperation(L"Boss_Gun_Effect", EFFECT_INFO(*mSuperCube_Info.pMyPos,
			D3DXVECTOR3(rand() % 2 - 1.f, rand() % 4 - 2.f, 0.f), D3DXVECTOR3(5.f, 5.f, 5.f)));
		m_pEffectMgr->EffectOperation(L"Boss_Sword_Effect", EFFECT_INFO(*mSuperCube_Info.pMyPos,
			D3DXVECTOR3(rand() % 2 - 1.f, rand() % 4 - 2.f, 0.f), D3DXVECTOR3(5.f, 5.f, 5.f)));
	}

	if (*mSuperCube_Info.pHP <= 0)
	{
		*mSuperCube_Info.pHP = 0;
		m_pHpBar->SetHp(*mSuperCube_Info.pHP);
		return false;
	}

	m_pHpBar->SetHp(*mSuperCube_Info.pHP);
	return true;
}

Boss * Boss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Boss* pInstance = new Boss(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Boss Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Boss::Clone()
{
	Boss* pInstance = new Boss(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Boss Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Boss::Free(void)
{
	Engine::Safe_Release(m_pHpBar); //TY_HP_180625 Release HpBar Object
	Engine::Safe_Release(m_pSphere);

	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pAnimMgr);
	Engine::Safe_Release(m_pEffectMgr);

	for (size_t i = 0; i < END; i++)
	{
		Engine::Safe_Release(m_pBossAI[i]);
	}

	GameObject::Free();

	return DWORD();
}
