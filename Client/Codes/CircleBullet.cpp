#include "stdafx.h"
#include "CircleBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

CircleBullet::CircleBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
CircleBullet::CircleBullet(const CircleBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT CircleBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	*m_matBulletWorld *= matScale;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mBulletInfo.vPos);
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);

	m_vCenterPos = *m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);

	D3DXMatrixTranslation(&m_matParent, m_vCenterPos.x, m_vCenterPos.y, m_vCenterPos.z);
	m_vCenterPos = { 0.f, 0.f, 0.f }; // 공전이므로 다시 원점 초기화

	mBulletInfo.speed = 5.f;
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));

	mRotRadian = D3DXToRadian(mRotDir *45.f);

	m_vCenterDir = { cosf(mRotRadian), 0.f, sinf(mRotRadian) };
	D3DXVec3Normalize(&m_vCenterDir, &m_vCenterDir);

	return NOERROR;
}

UPDATE CircleBullet::Update(const float timeDelta)
{
	m_vCenterPos += m_vCenterDir*mTimeDelta*mBulletInfo.speed;

	mRevolRadian += D3DXToRadian(45.f*timeDelta);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE CircleBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	mTimeDelta = timeDelta;

	if (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f)
	{
		return UPDATE::DIE;
	}

	if (mDead == UPDATE::DIE)
		return mDead;

	SetUpWorld();

	return Engine::GameObject::LastUpdate(timeDelta);
}

void CircleBullet::Render()
{
	if (nullptr == m_pBuffer)
		return;

	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();

	//TY_Sphere_180618 Render Sphere
	m_pSphere->Render();
}

HRESULT CircleBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NOERROR;
}

void CircleBullet::SetUpWorld()
{
	D3DXMATRIX	matScale, matRot, matTrans, matRevol;
	D3DXQUATERNION qRot;

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	D3DXMatrixTranslation(&matTrans, m_vCenterPos.x, m_vCenterPos.y, m_vCenterPos.z);
	D3DXMatrixRotationY(&matRevol, mRevolRadian);


	*m_matBulletWorld = matScale * matTrans * matRevol * m_matParent;
}

Bullet * CircleBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CircleBullet* pInstance = new CircleBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * CircleBullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag, const int dir)
{
	CircleBullet*	pInstance = new CircleBullet(*this);
	pInstance->ReadyBulletType(bulletInfo);
	pInstance->m_pSphereTag = pSphereTag;
	pInstance->mTextureNum = bulletInfo.textureNum;
	pInstance->mRotDir = dir;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Bullet Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD CircleBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
