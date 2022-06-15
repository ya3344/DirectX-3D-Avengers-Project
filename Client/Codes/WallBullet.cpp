#include "stdafx.h"
#include "WallBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

WallBullet::WallBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
WallBullet::WallBullet(const WallBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT WallBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	*m_matBulletWorld *= matScale;

	//TY_ChangeStage_180705_ Change BulletInfo 
	D3DXVECTOR3 vPos = { mBulletInfo.vPos.x, mBulletInfo.vPos.y, mBulletInfo.vPos.z };

	if (gTerrainState == TERRAIN_STATE::STAGE2_PLAY)
	{
		vPos.y -= 0.5f;
	}

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &vPos);
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);

	mBulletInfo.speed = 5.f;
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));

	mRim = 2.f;

	return NOERROR;
}

UPDATE WallBullet::Update(const float timeDelta)
{
	// TY_Bullet, Sphere Fix_180703_ Multiply timeDelta
	m_pTransform->GoStraight(mBulletInfo.speed * timeDelta);
	return Engine::GameObject::Update(timeDelta);
}

UPDATE WallBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	if (m_pMyPos->x <= -mRim || m_pMyPos->x >= 30.f + mRim || m_pMyPos->z <= -mRim || m_pMyPos->z >= 30.f + mRim)
	{
		return UPDATE::DIE;
	}

	if (mDead == UPDATE::DIE)
	{
		return mDead;
	}

	return Engine::GameObject::LastUpdate(timeDelta);
}

void WallBullet::Render()
{
	if (nullptr == m_pBuffer)
		return;

	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();

	//TY_Sphere_180618 Render Sphere
	m_pSphere->Render();
}

HRESULT WallBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NO_ERROR;
}

Bullet * WallBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	WallBullet* pInstance = new WallBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * WallBullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag)
{
	WallBullet*	pInstance = new WallBullet(*this);
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

DWORD WallBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
