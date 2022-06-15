#include "stdafx.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "AnimCube.h"
#include "Layer.h"
#include "Sphere.h"
#include "HpBar.h"
#include "ShieldBullet.h"

_USING(Client)

Player::Player(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
	, m_pEffectMgr(EffectMgr::GetInstance())
{
	m_pEffectMgr->AddRef();
}

Player::Player(const Player & rhs)
	: Engine::GameObject(rhs)
	, mSuperCube_Info(rhs.mSuperCube_Info)
	, m_pEffectMgr(rhs.m_pEffectMgr)
{
	mSuperCube_Info.pCube->AddRef();
	mSuperCube_Info.pAnimNode->AddRef();
	mSuperCube_Info.pTransform->AddRef();
	m_pEffectMgr->AddRef();
}


HRESULT Player::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pAnimMgr->Set_AnimLayer(Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_PLAYER, L"Layer_Player_AnimCube"));

	if (FAILED(m_pAnimMgr->SetHierachy()))
	{
		_MSG_BOX("Player Hierachy Failed!");
	}

	// DH_BulletAdd_180618  PlayerInfo Setting
	BulletMgr::GetInstance()->SetPlayerInfo(&mSuperCube_Info);
	//mSuperCube_Info.pAngle = 

	//TY_HP_180628 Initialize Hp
	mSuperCube_Info.pHP = &mHP;
	mSuperCube_Info.pMaxHP = &mMaxHP;

	*mSuperCube_Info.pHP = 3.f;
	*mSuperCube_Info.pMaxHP = 3.f;
	m_pHpBar->SetHp(*mSuperCube_Info.pHP);

	return NOERROR;
}
#include "CameraMgr.h"
UPDATE Player::Update(const float timeDelta)
{
	if(m_pKeyMgr->KeyDown(DIK_L))
		CameraMgr::GetInstance()->ShakeCamera(1.f);

	KeyCheck(timeDelta);
	AnimStateChange();
	m_pAnimMgr->InterpolationUpdate(timeDelta);
	AnimUpdate();

	if (mSuperCube_Info.pAnimNode != nullptr)
	{
		mSuperCube_Info.pAnimNode->RecursiveMatrix();
	}

	m_pShield->Update(timeDelta);

	// TY_Sliding_180702_Call SlidingWall Function
	SlidingWall();

	return Engine::GameObject::Update(timeDelta);
}

UPDATE Player::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	m_pShield->LastUpdate(timeDelta);

	return Engine::GameObject::LastUpdate(timeDelta);
}

void Player::Render()
{
	m_pAnimMgr->Render();
	m_pSphere->Render();

	//TY_HP_180627 Render HpBar
	m_pHpBar->Render();
	m_pShield->Render();
}

HRESULT Player::ReadyPrototype()
{
	if (FAILED(Ready_LayerAnimCube()))
	{
		_MSG_BOX("Palyer FileLoad Failed!");
		return E_FAIL;
	}

	return NOERROR;
}

HRESULT Player::Ready_LayerAnimCube()
{
	// 슈퍼부모 속성 받아오기
	AnimCube* pSuperCube = (AnimCube*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PLAYER, L"Layer_Player_AnimCube", L"Waist");

	// 슈퍼부모 속성 적용
	Set_SuperCube(pSuperCube);

	return NOERROR;
}

HRESULT Player::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_AnimMgr
	pComponent = m_pAnimMgr = (AnimMgr*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_AnimMgr");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_AnimMgr", pComponent)))
		return E_FAIL;

	//TY_Sphere_180618 Add Sphere Object
	Engine::GameObject* pGameObject = m_pSphere = Sphere::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	m_pSphere->AddRef();

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Player_Sphere", SCENE_STAGE, m_pSphere)))
	{
		_MSG_BOX("Add Sphere Failed");
	}

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_Player_Sphere", SCENE_STAGE, L"Layer_Sphere")))
	{
		_MSG_BOX("Add Sphere Failed");
	}

	m_pSphere->SetGameObject(mSuperCube_Info.pTransform);
	m_pSphere->SetRadius(0.7f); // TY_Bullet, Sphere Fix_180702_Change Radius of Sphere

	//TY_HP_180626 Add HpBar
	pGameObject = m_pHpBar = HpBar::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	m_pHpBar->AddRef();

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Player_HpBar", SCENE_STAGE, m_pHpBar)))
	{
		_MSG_BOX("Add HpBar Failed");
	}

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_Player_HpBar", SCENE_STAGE, L"Layer_HpBar")))
	{
		_MSG_BOX("Add HpBar Failed");
	}

	m_pHpBar->SetGameObject(mSuperCube_Info.pTransform);
	m_pHpBar->SetSize(D3DXVECTOR3{ 3.f, 0.4f, 0.4f });
	m_pHpBar->SetHeight(3.f);

	//TY_FixBullet_180702_Add ShieldBullet 
	Bullet* pPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, L"Prototype_GameObject_ShieldBullet");
	if (pPrototype == nullptr)
		return E_FAIL;

	m_pShield = dynamic_cast<ShieldBullet*>(pPrototype)->Clone(BULLET_INFO(SCENE_BULLET, L"Prototype_GameObject_ShieldBullet", 15, *mSuperCube_Info.pMyPos, *mSuperCube_Info.pTransform->GetAngle()), L"player_ShieldSphere");

	return NOERROR;
}

HRESULT Player::KeyCheck(const float timeDelta)
{
	if (mAnimState == ANIM_STATE::WALK)
	{
		mAnimState = ANIM_STATE::STAND;
	}

	if (m_pKeyMgr->KeyPressing(DIK_UP))
	{
		mSuperCube_Info.pTransform->GoStraight(6.0f * timeDelta);
		mAnimState = ANIM_STATE::WALK;
	}
	
	if (m_pKeyMgr->KeyPressing(DIK_DOWN))
	{
		mSuperCube_Info.pTransform->GoStraight(-6.0f * timeDelta);
		mAnimState = ANIM_STATE::WALK;
	}

	if (m_pKeyMgr->KeyPressing(DIK_LEFT))
	{
		mSuperCube_Info.pTransform->RotationUp(D3DXToRadian(-90.0f) * timeDelta);
	}

	if (m_pKeyMgr->KeyPressing(DIK_RIGHT))
	{
		mSuperCube_Info.pTransform->RotationUp(D3DXToRadian(90.0f) * timeDelta);
	}

	if (m_pKeyMgr->KeyDown(DIK_SPACE))
	{
		mAnimState = ANIM_STATE::ATTACK0;
	}

	//TY_ChangeStage_180706_ 시연용 치트키
	if (m_pKeyMgr->KeyDown(DIK_F3))
	{
		m_pShield->SetActive(false);
		m_pShield->SetEquip(true);
		SetShield_RenderState(true);
	}

	return NOERROR;
}

void Player::SlidingWall()
{
	D3DXVECTOR3 vTmpPos;
	if (mSuperCube_Info.pMyPos->x < 1.f) // 왼쪽
	{
		//(D3DXVECTOR3{ 1.f, 0.f, 0.f });
		vTmpPos = *mSuperCube_Info.pMyPos;
		mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ 1.1f, vTmpPos.y, vTmpPos.z });
	}
	else if (mSuperCube_Info.pMyPos->x > 28.f) // 오른쪽
	{
		//Make_SlidingVector(D3DXVECTOR3{ -1.f, 0.f, 0.f });
		vTmpPos = *mSuperCube_Info.pMyPos;
		mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ 27.9f, vTmpPos.y, vTmpPos.z });
	}
	else if (mSuperCube_Info.pMyPos->z < 1.f) // 하단
	{
		//Make_SlidingVector(D3DXVECTOR3{ 0.f, 0.f, 1.f });
		vTmpPos = *mSuperCube_Info.pMyPos;
		mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ vTmpPos.x, vTmpPos.y, 1.1f });
	}
	else if (mSuperCube_Info.pMyPos->z > 28.f) // 상단
	{
		//Make_SlidingVector(D3DXVECTOR3{ 0.f, 0.f, -1.f });
		vTmpPos = *mSuperCube_Info.pMyPos;
		mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ vTmpPos.x, vTmpPos.y, 27.9f });
	}
}

void Player::AnimStateChange()
{
	if (mPreAnimState != mAnimState)
	{
		m_pFrameInfo = nullptr;
		m_pAnimInfo = nullptr;

		switch (mAnimState)
		{
		case ANIM_STATE::STAND:
			InitAnimState(L"Stand", L"LA_Joint", D3DXVECTOR3(1.f, 1.f, 1.f));
			break;
		case ANIM_STATE::WALK:
			InitAnimState(L"Walk", L"LA_Joint", D3DXVECTOR3(2.f, 2.f, 2.f));
			break;
		case ANIM_STATE::ATTACK0:
			InitAnimState(L"Attack0", L"LA_Joint", D3DXVECTOR3(1.f, 3.f, 1.f));
			break;
		case ANIM_STATE::END:
			break;
		default:
			assert(false && "Player::AnimStateChange");
			break;
		}

		mPreAnimState = mAnimState;
	}

}

void Player::AnimUpdate()
{
	switch (mAnimState)
	{
	case ANIM_STATE::STAND:
		break;
	case ANIM_STATE::WALK:
		break;
	case ANIM_STATE::ATTACK0:
		// TY_FixBullet_180702_ Add Key Check For Shield Bullet
		if (m_pFrameInfo != nullptr)
		{
			if (m_pFrameInfo->rotFrame >= 3)
			{
				Attack();
			}
		}
		ChangeStandAnim();
		break;
	case ANIM_STATE::END:
		break;
	}
}

void Player::ChangeStandAnim()
{
	if (m_pFrameInfo != nullptr)
	{
		if (m_pFrameInfo->rotFrame >= m_pAnimInfo->cubeInfo_Space.size() - 1)
		{
			mAnimState = ANIM_STATE::STAND;
		}
	}
}

void Player::InitAnimState(const TCHAR * pAnimName, const TCHAR * pCubeName, const D3DXVECTOR3 vAnimSpeed)
{
	m_pAnimMgr->Set_InitRot(L"Stand");
	m_pAnimMgr->Set_AnimInfo(pAnimName);
	m_pAnimMgr->Set_AnimSpeed(vAnimSpeed.x, vAnimSpeed.y, vAnimSpeed.z);
	m_pAnimMgr->Set_FrameInfo(FRAME_INFO(0, 0, 0, true));

	Set_FrameInfo(pCubeName);
	Set_AnimInfo(pCubeName);
}

void Player::Set_SuperCube(AnimCube* pAnimCube)
{
	if (pAnimCube == nullptr)
	{
		_MSG_BOX("Player Set SuperCube Failed!");
		return;
	}

	mSuperCube_Info.pCube = pAnimCube;
	mSuperCube_Info.pCube->AddRef();

	mSuperCube_Info.pAnimNode = mSuperCube_Info.pCube->Get_AnimNode();
	mSuperCube_Info.pAnimNode->AddRef();

	mSuperCube_Info.pTransform = mSuperCube_Info.pCube->GetTransform();
	mSuperCube_Info.pTransform->AddRef();

	mSuperCube_Info.pMyPos = mSuperCube_Info.pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	mSuperCube_Info.pMyLook = mSuperCube_Info.pTransform->GetInfo(Engine::Transform::INFO_LOOK);

	// Initialize 
	//TY_ChangeStage_180704_ Set Player Position
#ifdef _TERRAIN_INIT_DEBUG
	mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(15.f, 1.6f, 2.f));
#else
	mSuperCube_Info.pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(15.f, 100.f, 2.f));
#endif
}

void Player::SetDamage(const float hp)
{
	// JH_Effect_180704 HitEffect 추가

	//TY_HP_180628 Set Hp
	*mSuperCube_Info.pHP -= hp;
	m_pHpBar->SetHp(*mSuperCube_Info.pHP);

	m_pEffectMgr->EffectOperation(L"Player_Hit_Effect", EFFECT_INFO(*mSuperCube_Info.pMyPos,
		D3DXVECTOR3(rand() % 2 - 0.25f, rand() % 2 + 0.25f, 0.f), D3DXVECTOR3(2.f, 2.f, 2.f)));

	if (*mSuperCube_Info.pHP <= 0.f)
	{
		m_pHpBar->AddDeathCount();
		*mSuperCube_Info.pHP = *mSuperCube_Info.pMaxHP;
		m_pHpBar->SetHp(*mSuperCube_Info.pHP);
	}
}

void Player::Attack()
{
	if (m_pShield->IsEquip() && !m_pShield->IsActive())
	{
		BULLET_INFO bulletInfo;
		bulletInfo.angle = *mSuperCube_Info.pTransform->GetAngle();
		bulletInfo.vPos = *mSuperCube_Info.pMyPos;

		m_pShield->ResetShield(bulletInfo);
		m_pShield->SetActive(true);
		SetShield_RenderState(true);
		Engine::SoundMgr::GetInstance()->SoundPlay(L"uppershield.ogg", Engine::SoundMgr::EFFECT);
	}
}

void Player::SetShield_RenderState(const bool renderState)
{
	if (m_pAnimMgr != nullptr)
	{
		m_pAnimMgr->SetShiled_RenderState(renderState);
	}
}

Player * Player::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Player* pInstance = new Player(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Player Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Player::Clone()
{
	Player*	pInstance = new Player(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Player Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Player::Free(void)
{
	Engine::Safe_Release(m_pSphere);
	Engine::Safe_Release(m_pHpBar); //TY_HP_180626 Release HpBar Object
	Engine::Safe_Release(m_pShield); //TY_FixBullet_180701_Release Shield Object

	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pAnimMgr);

	Engine::Safe_Release(mSuperCube_Info.pCube);
	Engine::Safe_Release(mSuperCube_Info.pTransform);
	Engine::Safe_Release(mSuperCube_Info.pAnimNode);

	Engine::Safe_Release(m_pEffectMgr);


	GameObject::Free();

	return DWORD();
}
