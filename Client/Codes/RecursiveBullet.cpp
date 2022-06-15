#include "stdafx.h"
#include "RecursiveBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

RecursiveBullet::RecursiveBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
RecursiveBullet::RecursiveBullet(const RecursiveBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT RecursiveBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	*m_matBulletWorld *= matScale;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mBulletInfo.vPos);
	//m_pTransform->SetRotation(D3DXToRadian(*m_pObjInfo->pTransform->GetAngle()));
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);


	mBulletInfo.speed = 0.5f;
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));

	//TY_FixChangeTerrain_180704 Set Bullet Pos
	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, m_pMyPos);

	mTimeAcc = 0.f;
	mTimeDead = (rand() / 300) * 0.01f + 0.6f;
	mPaceAcc = 0.f;

	return NOERROR;
}

UPDATE RecursiveBullet::Update(const float timeDelta)
{
	mTimeAcc += timeDelta;

	mPaceAcc += timeDelta;
	mBulletInfo.speed += mPaceAcc;
	m_pTransform->GoStraight(mBulletInfo.speed*timeDelta);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE RecursiveBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	if (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f)
	{
		return UPDATE::DIE;
	}
	else if (mTimeAcc >= mTimeDead
		&& mRecursiveCount <= 3)
	{
		return UPDATE::BULLET_BOMB;
	}
	else if (mDead == UPDATE::DIE)
	{
		return mDead;
	}

	return Engine::GameObject::LastUpdate(timeDelta);
}

void RecursiveBullet::Render()
{
	if (nullptr == m_pBuffer)
		return;

	m_pTexture->SetUp_Device(mTextureNum);
	//m_pTransform->SetUp_World();
	m_pDevice->SetTransform(D3DTS_WORLD, m_matBulletWorld);
	m_pBuffer->Render();

	//TY_Sphere_180618 Render Sphere
	m_pSphere->Render();
}

HRESULT RecursiveBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NO_ERROR;
}

Bullet * RecursiveBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	RecursiveBullet* pInstance = new RecursiveBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * RecursiveBullet::Clone(const BULLET_INFO & bulletInfo, const TCHAR * pSphereTag, const int recursiveCount)
{
	RecursiveBullet*	pInstance = new RecursiveBullet(*this);
	pInstance->ReadyBulletType(bulletInfo);
	pInstance->m_pSphereTag = pSphereTag;
	pInstance->mRecursiveCount = recursiveCount;
	pInstance->mTextureNum = bulletInfo.textureNum;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Bullet Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD RecursiveBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
