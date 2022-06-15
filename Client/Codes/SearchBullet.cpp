#include "stdafx.h"
#include "SearchBullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

SearchBullet::SearchBullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
SearchBullet::SearchBullet(const SearchBullet & rhc)
	: Bullet(rhc)
{
}

HRESULT SearchBullet::Init()
{
	D3DXMATRIX		matScale;

	if (FAILED(ReadyComponent()))
		return E_FAIL;
	m_matBulletWorld = m_pTransform->GetWolrdPointer();

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);
	*m_matBulletWorld *= matScale;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mBulletInfo.vPos);
	m_pMyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);

	D3DXMATRIX matRot;
	m_vLook = { 0.f, 0.f, 1.f };
	D3DXMatrixRotationY(&matRot, D3DXToRadian(mBulletInfo.angle + float(rand() % 90 - 45)));
	D3DXVec3TransformNormal(&m_vLook, &m_vLook, &matRot);

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	m_vPos = *m_pMyPos;

	mBulletInfo.speed = 3.f;

	mIntertia = 20.f;

	return NOERROR;
}

UPDATE SearchBullet::Update(const float timeDelta)
{
	if (m_pTargetInfo == nullptr)
		return UPDATE::FAIL;

	D3DXVECTOR3 vInter = *m_pTargetInfo->pMyPos - m_vPos;
	float interLen = D3DXVec3Length(&vInter);
	D3DXVec3Normalize(&vInter, &vInter);

	m_vPos += (vInter * mBulletInfo.speed + m_vLook*mIntertia) * timeDelta;

	if (mIntertia > 0.f)
	{
		mIntertia -= 7.f * timeDelta;
		mBulletInfo.speed += 7.f * timeDelta;
	}
	mRotRadian += D3DXToRadian(45.f * timeDelta);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE SearchBullet::LastUpdate(const float timeDelta)
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

	SetUpWorld();

	return Engine::GameObject::LastUpdate(timeDelta);
}

void SearchBullet::Render()
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

HRESULT SearchBullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;

	return NO_ERROR;
}

void SearchBullet::SetUpWorld()
{

	D3DXMATRIX	matScale, matRot, matTrans;
	D3DXQUATERNION qRot;

	D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.6f);
	D3DXQuaternionRotationYawPitchRoll(&qRot, mRotRadian, mRotRadian, mRotRadian);
	D3DXMatrixRotationQuaternion(&matRot, &qRot);

	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	*m_matBulletWorld = matScale * matRot * matTrans;
}

Bullet * SearchBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	SearchBullet* pInstance = new SearchBullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * SearchBullet::Clone(const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo, const TCHAR* pSphereTag)
{
	SearchBullet*	pInstance = new SearchBullet(*this);
	pInstance->ReadyBulletType(bulletInfo);
	pInstance->m_pSphereTag = pSphereTag;
	pInstance->mTextureNum = bulletInfo.textureNum;

	pInstance->m_pTargetInfo = pTargetInfo;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Bullet Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD SearchBullet::Free(void)
{
	Bullet::Free();
	return 0;
}
