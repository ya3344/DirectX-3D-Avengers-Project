#include "stdafx.h"
#include "NormalBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

NormalBullet::NormalBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
NormalBullet::NormalBullet(const NormalBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT NormalBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);
	*m_matBulletWorld *= matScale;

	//TY_ChangeStage_180705_ Change BulletInfo 
	D3DXVECTOR3 vPos = { mBulletInfo.vPos.x, mBulletInfo.vPos.y, mBulletInfo.vPos.z };

	if (gTerrainState == TERRAIN_STATE::STAGE2_PLAY)
	{
		vPos.y -= 0.5f;
	}

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &vPos);
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);

	mBulletInfo.speed = 3.f;
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));

	return NOERROR;
}

UPDATE NormalBullet::Update(const float timeDelta)
{
	m_pTransform->GoStraight(mBulletInfo.speed * timeDelta);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE NormalBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	if (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f)
	{
		return UPDATE::BULLET_WALL;
	}

	if (mDead == UPDATE::DIE)
	{
		return mDead;
	}

	return Engine::GameObject::LastUpdate(timeDelta);
}

void NormalBullet::Render()
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

HRESULT NormalBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NO_ERROR;
}

Bullet * NormalBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	NormalBullet* pInstance = new NormalBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * NormalBullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag)
{
	NormalBullet*	pInstance = new NormalBullet(*this);
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

DWORD NormalBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
