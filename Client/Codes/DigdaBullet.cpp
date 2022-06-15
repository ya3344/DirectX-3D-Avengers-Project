#include "stdafx.h"
#include "DigdaBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

DigdaBullet::DigdaBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
DigdaBullet::DigdaBullet(const DigdaBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT DigdaBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	*m_matBulletWorld *= matScale;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mBulletInfo.vPos);
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);

	m_vCenterPos = *m_pTargetInfo->pMyPos;

	D3DXMatrixTranslation(&m_matParent, m_vCenterPos.x, m_vCenterPos.y, m_vCenterPos.z);
	m_vCenterPos = { 0.f, 0.f, 0.f }; // 공전이므로 다시 원점 초기화

	mBulletInfo.speed = 15.f;

	mRotRadian = D3DXToRadian(mRotDir *45.f);

	m_vCenterDir = { 0.f, 0.f,1.f };
	mDirDist = 60;
	//m_vCenterDir = { cosf(mRotRadian), 0.f, sinf(mRotRadian) };
	//D3DXVec3Normalize(&m_vCenterDir, &m_vCenterDir);

	m_vCenterPos = float(mDirDist)* m_vCenterDir*mBulletInfo.speed * 0.01f;
	m_vCenterPos.y = 0.5f;

	return NOERROR;
}

UPDATE DigdaBullet::Update(const float timeDelta)
{
	mDigdaTime += timeDelta;

	if (mDigdaTime < 0.02f)
	{
		m_vCenterPos.y += mBulletInfo.speed * timeDelta*2.f;
	}
	else if (mDigdaTime < 0.04f)
	{
		m_vCenterPos.y -= mBulletInfo.speed * timeDelta*2.f;
	}
	else
	{
		m_vCenterPos = float(mDirDist)* m_vCenterDir*mBulletInfo.speed * 0.01f;
		m_vCenterPos.y += 0.5f;

		--mDirDist;
		mDigdaTime = 0;
	}

	return Engine::GameObject::Update(timeDelta);
}

UPDATE DigdaBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;


	if (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f
		|| mDirDist == 0)
	{
		return UPDATE::DIE;
	}

	if (mDead == UPDATE::DIE)
		return mDead;

	SetUpWorld();

	return Engine::GameObject::LastUpdate(timeDelta);
}

void DigdaBullet::Render()
{
	if (nullptr == m_pBuffer)
		return;

	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();

	//TY_Sphere_180618 Render Sphere
	m_pSphere->Render();
}

HRESULT DigdaBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NOERROR;
}

void DigdaBullet::SetUpWorld()
{
	D3DXMATRIX	matScale, matRot, matTrans, matRevol;
	D3DXQUATERNION qRot;

	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.8f);
	//TY_ChangeStage_180705_ Change Y value
	D3DXMatrixTranslation(&matTrans, m_vCenterPos.x, m_vCenterPos.y - 1.f, m_vCenterPos.z);
	D3DXMatrixRotationY(&matRevol, mRotRadian);


	*m_matBulletWorld = matScale * matTrans * matRevol * m_matParent;
}

Bullet * DigdaBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DigdaBullet* pInstance = new DigdaBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * DigdaBullet::Clone(const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo, const TCHAR* pSphereTag, const int dir)
{
	DigdaBullet*	pInstance = new DigdaBullet(*this);
	pInstance->ReadyBulletType(bulletInfo);
	pInstance->m_pSphereTag = pSphereTag;
	pInstance->mTextureNum = bulletInfo.textureNum;
	pInstance->mRotDir = dir;
	pInstance->m_pTargetInfo = pTargetInfo;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Bullet Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD DigdaBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
