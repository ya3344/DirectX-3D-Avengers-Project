#include "stdafx.h"
#include "..\Headers\BuckBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

BuckBullet::BuckBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
BuckBullet::BuckBullet(const BuckBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT BuckBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	*m_matBulletWorld *= matScale;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mBulletInfo.vPos);
	m_pTransform->SetRotation(D3DXToRadian(mBulletInfo.angle));
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);


	mBulletInfo.speed = 0.3f;
	m_pTransform->RotationUp(D3DXToRadian(rand() % 25));
	m_pTransform->RotationUp(-D3DXToRadian(rand() % 25));


	return NOERROR;
}

UPDATE BuckBullet::Update(const float timeDelta)
{
	m_pTransform->GoStraight(mBulletInfo.speed);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE BuckBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	if (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f)
	{

		return UPDATE::DIE;
	}

	if (mDead == UPDATE::DIE)
	{
		return mDead;
	}

	//D3DXMATRIX		matRotation;

	//mAngle += 720.f * timeDelta;
	//D3DXMatrixRotationY(&matRotation, D3DXToRadian(mAngle));
	//*m_matBulletWorld = matRotation * m_pTransform->GetWolrd();

	return Engine::GameObject::LastUpdate(timeDelta);
}

void BuckBullet::Render()
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

HRESULT BuckBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NO_ERROR;
}

Bullet * BuckBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	BuckBullet* pInstance = new BuckBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * BuckBullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag)
{
	BuckBullet*	pInstance = new BuckBullet(*this);
	pInstance->ReadyBulletType(bulletInfo);
	pInstance->mTextureNum = bulletInfo.textureNum;
	pInstance->m_pSphereTag = pSphereTag;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Bullet Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD BuckBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
