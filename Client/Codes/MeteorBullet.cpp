#include "stdafx.h"
#include "MeteorBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"
#include "CameraMgr.h"

_USING(Client)

MeteorBullet::MeteorBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
MeteorBullet::MeteorBullet(const MeteorBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT MeteorBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	*m_matBulletWorld *= matScale;

	mBulletInfo.vPos += {float(rand() % 10), 0.f, float(rand() % 10)};
	mBulletInfo.vPos.y += 20.f;
	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mBulletInfo.vPos);

	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_vPos = { float(rand() % 30 - 15), m_pMyPos->y + rand() % 30, 15.f + m_pMyPos->z + float(rand() % 30 - 15) };
	m_vLook = { (rand() % 20)*0.02f, -1.f, -(rand() % 20)*0.02f };
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	mBulletInfo.speed = float(rand() % 25 + 45);
	accPace = 0.f;

	return NOERROR;
}

UPDATE MeteorBullet::Update(const float timeDelta)
{
	SetUpWorld();

	return Engine::GameObject::Update(timeDelta);
}

UPDATE MeteorBullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	if (m_pMyPos->y <= -29.f)
	{
		if (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f)
		{
			return UPDATE::DIE;
		}
		else
		{
			//TY_ChangeStage_180705_ ShakeCamera
			CameraMgr::GetInstance()->ShakeCamera(1.f);
			mCollisionTime += timeDelta;
		}
	}
	if (mDead == UPDATE::DIE)
	{
		return mDead;
	}

	if (mCollisionTime > 0.f && mCollisionTime < 1.f)
	{
		return UPDATE::NORMAL;
	}
	else if (mCollisionTime > 1.f)
	{
		return UPDATE::BULLET_METEOR;
	}

	accPace += 2.f * timeDelta;
	m_vPos += m_vLook * mBulletInfo.speed * accPace * timeDelta;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void MeteorBullet::Render()
{
	if (nullptr == m_pBuffer)
		return;

	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();

	//TY_Sphere_180618 Render Sphere
	m_pSphere->Render();
}

HRESULT MeteorBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NO_ERROR;
}

void MeteorBullet::SetUpWorld()
{
	if (mCollisionTime != 0.f)
		return;

	D3DXMATRIX	matScale, matRot, matTrans;
	D3DXQUATERNION qRot;

	mRotRadian += D3DXToRadian(10.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXQuaternionRotationYawPitchRoll(&qRot, mRotRadian, mRotRadian, mRotRadian);
	D3DXMatrixRotationQuaternion(&matRot, &qRot);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	*m_matBulletWorld = matScale * /*matRot * */matTrans;
}

Bullet * MeteorBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	MeteorBullet* pInstance = new MeteorBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * MeteorBullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag)
{
	MeteorBullet*	pInstance = new MeteorBullet(*this);
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

DWORD MeteorBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
