#include "stdafx.h"
#include "ShieldBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

ShieldBullet::ShieldBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
ShieldBullet::ShieldBullet(const ShieldBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT ShieldBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	m_vPos = mBulletInfo.vPos;

	mBulletInfo.speed = 80.f;
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));
	m_vLook = *m_pTransform->GetInfo(Engine::Transform::INFO_LOOK);

	mRot = 0.f;
	mRotAcc = D3DXToRadian(45.f);
	mRotDir = 1;

	m_bEquip = true;
	m_bActive = false;

	m_pSphere->SetRadius(0.5f);

	return NOERROR;
}

UPDATE ShieldBullet::Update(const float timeDelta)
{
	if (!m_bActive)
		return UPDATE::NORMAL;

	// 속도 
	if (mBulletInfo.speed > 0.1f)
	{
		mBulletInfo.speed -= 15.f * timeDelta;


		// 높이
		if (m_vPos.y > 1.f)
		{
			m_vPos.y -= 0.3f * timeDelta;
		}
	}
	else
	{
		mBulletInfo.speed = 0.f;
		m_bEquip = false;
	}

	// 회전	
	mRot += mRotAcc;
	if (mRotAcc > 0.f)
	{
		mRotAcc -= D3DXToRadian(8.f)*timeDelta;
	}
	else
		mRotAcc = 0.f;

	return Engine::GameObject::Update(timeDelta);
}

UPDATE ShieldBullet::LastUpdate(const float timeDelta)
{
	if (!m_bActive)
		return UPDATE::NORMAL;

	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	if (m_vPos.x < 1.f) // 왼쪽
	{
		if (m_vPos.z > 29.f) // 꼭지점 상단
		{
			mBulletInfo.speed = 0.f;
			m_vPos = D3DXVECTOR3{ 1.f, m_vPos.y, 29.f };
		}
		else if (m_vPos.z < 1.f) // 꼭지점 하단
		{
			mBulletInfo.speed = 0.f;
			m_vPos = D3DXVECTOR3{ 1.f, m_vPos.y, 1.f };
		}
		m_pSoundMgr->SoundPlay(L"bowgun_metalhit_02.ogg", Engine::SoundMgr::EFFECT);
		ReflectWall(D3DXVECTOR3{ 1.f, 0.f, 0.f });
		mRotDir *= -1;
	}
	else if (m_vPos.x > 28.f) // 오른쪽
	{
		if (m_vPos.z > 28.f) // 꼭지점 상단
		{
			mBulletInfo.speed = 0.f;
			m_vPos = D3DXVECTOR3{ 28.f, m_vPos.y, 28.f };
		}
		else if (m_vPos.z < 1.f) // 꼭지점 하단
		{
			mBulletInfo.speed = 0.f;
			m_vPos = D3DXVECTOR3{ 28.f, m_vPos.y, 1.f };
		}
		m_pSoundMgr->SoundPlay(L"bowgun_metalhit_02.ogg", Engine::SoundMgr::EFFECT);
		ReflectWall(D3DXVECTOR3{ -1.f, 0.f, 0.f });
		mRotDir *= -1;
	}
	else if (m_vPos.z < 1.f) // 하단
	{
		ReflectWall(D3DXVECTOR3{ 0.f, 0.f, 1.f });
		m_pSoundMgr->SoundPlay(L"bowgun_metalhit_02.ogg", Engine::SoundMgr::EFFECT);

		mRotDir *= -1;
	}
	else if (m_vPos.z > 29.f) // 상단
	{
		m_pSoundMgr->SoundPlay(L"bowgun_metalhit_02.ogg", Engine::SoundMgr::EFFECT);
		ReflectWall(D3DXVECTOR3{ 0.f, 0.f, -1.f });
		mRotDir *= -1;
	}

	if (mDead == UPDATE::DIE)
	{
		return mDead;
	}
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	m_vPos += m_vLook * mBulletInfo.speed * timeDelta;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void ShieldBullet::Render()
{
	m_pSphere->Render();

	if (!m_bActive)
		return;

	if (nullptr == m_pBuffer)
		return;

	SetUpWorld(D3DXVECTOR3{ 1.f, 0.4f, 2.f });
	m_pTransform->SetUp_World();
	m_pTexture->SetUp_Device(mTextureNum);
	m_pBuffer->Render();


	SetUpWorld(D3DXVECTOR3{ 2.f, 0.4f, 1.f });
	m_pTransform->SetUp_World();
	m_pTexture->SetUp_Device(mTextureNum);
	m_pBuffer->Render();

	SetUpWorld(D3DXVECTOR3{ 1.5f, 0.6f, 1.5f });
	m_pTransform->SetUp_World();
	m_pTexture->SetUp_Device(mTextureNum);
	m_pBuffer->Render();
}

HRESULT ShieldBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NOERROR;
}

void ShieldBullet::ReflectWall(D3DXVECTOR3 vWallSide)
{
	D3DXVECTOR3 vWall_Normal = vWallSide;
	m_vLook *= -1.f;

	float length;
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	length = D3DXVec3Dot(&vWall_Normal, &m_vLook);
	vWall_Normal *= 2 * length;

	m_vLook = vWall_Normal - m_vLook;
}

void ShieldBullet::SetUpWorld(const D3DXVECTOR3& vScale)
{
	D3DXMATRIX matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationY(&matRot, mRot * mRotDir);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	*m_matBulletWorld = matScale * matRot * matTrans;
}

HRESULT ShieldBullet::ResetShield(const BULLET_INFO & bulletInfo)
{
	mBulletInfo = bulletInfo;

	D3DXMATRIX		matScale;

	m_vPos = mBulletInfo.vPos;

	mBulletInfo.speed = 80.f;
	m_pTransform->SetRotation(0.f);
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));
	m_vLook = *m_pTransform->GetInfo(Engine::Transform::INFO_LOOK);

	mRot = 0.f;
	mRotAcc = D3DXToRadian(45.f);

	m_bEquip = true;
	m_bActive = true;

	return NOERROR;
}

Bullet * ShieldBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	ShieldBullet* pInstance = new ShieldBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

ShieldBullet * ShieldBullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag)
{
	ShieldBullet*	pInstance = new ShieldBullet(*this);
	pInstance->ReadyBulletType(bulletInfo);
	pInstance->m_pSphereTag = pSphereTag;
	pInstance->mTextureNum = bulletInfo.textureNum;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Bullet Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD ShieldBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
