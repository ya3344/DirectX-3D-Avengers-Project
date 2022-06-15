#include "stdafx.h"
#include "DNABullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

DNABullet::DNABullet(LPDIRECT3DDEVICE9 pDevice)
	: Bullet(pDevice)
{
}
DNABullet::DNABullet(const DNABullet & rhc)
	: Bullet(rhc)
{
}

HRESULT DNABullet::Init()
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

	m_vCenterPos = *m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_vCenterPos.y += 1.f; // 땅에 쳐박히지 않기 위해
	m_vCenterDir = D3DXVECTOR3{ sinf(D3DXToRadian(mBulletInfo.angle)), 0.f,  cosf(D3DXToRadian(mBulletInfo.angle)) };
	D3DXVec3Normalize(&m_vCenterDir, &m_vCenterDir);

	mBulletInfo.speed = 0.1f;
	m_pTransform->RotationUp(D3DXToRadian(mBulletInfo.angle));

	mRange = 1.f;

	// DNA Bullet에서 가운데 위치한 큐브
	if (mRotDir == 0)
	{
		m_bDNACenter = true;
	}

	return NOERROR;
}

UPDATE DNABullet::Update(const float timeDelta)
{
	//m_pTransform->GoStraight(mBulletInfo.speed);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE DNABullet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	mTimeDelta = timeDelta;

	if (m_bWallCollision == false) // 벽에 처음 부딪혔을 때 
	{
		if (m_pMyPos->x <= 0.f) // 왼쪽
		{
			m_vCenterDir = D3DXVECTOR3{ 1.f, 0.f, float(rand() % 20)*0.1f - 1.f };
			m_bWallCollision = true;
		}
		else if (m_pMyPos->x >= 30.f) // 오른쪽
		{
			m_vCenterDir = D3DXVECTOR3{ -1.f, 0.f, float(rand() % 20)*0.1f - 1.f };
			m_bWallCollision = true;
		}
		else if (m_pMyPos->z <= 0.f) // 하단
		{
			m_vCenterDir = D3DXVECTOR3{ float(rand() % 20)*0.1f - 1.f, 0.f, 1.f };
			m_bWallCollision = true;
		}
		else if (m_pMyPos->z >= 30.f) // 상단
		{
			m_vCenterDir = D3DXVECTOR3{ float(rand() % 20)*0.1f - 1.f, 0.f, -1.f };
			m_bWallCollision = true;
		}
	} // 벽에 두번째로 부딪혔을 때
	else if (m_bWallCollision == true
		&& (m_pMyPos->x <= 0.f || m_pMyPos->x >= 30.f || m_pMyPos->z <= 0.f || m_pMyPos->z >= 30.f))
	{
		return UPDATE::DIE;
	}

	if (mDead == UPDATE::DIE)
		return mDead;

	if (m_bWallCollision == false || m_bDNACenter == false)
		SetUpWorld_DNA();
	else
		SetUpWorld_Normal();

	return Engine::GameObject::LastUpdate(timeDelta);
}

void DNABullet::Render()
{
	if (nullptr == m_pBuffer)
		return;

	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();

	//TY_Sphere_180618 Render Sphere
	m_pSphere->Render();
}

HRESULT DNABullet::ReadyBulletType(const BULLET_INFO& bulletInfo)
{
	mBulletInfo = bulletInfo;
	return NOERROR;
}

void DNABullet::SetUpWorld_DNA()
{
	if (mRange < -1.f || mRange > 1.f)
		m_bDir = !m_bDir;

	if (m_bDir)
		mRange -= 0.06f;
	else
		mRange += 0.06f;


	D3DXVECTOR3 vNormal = D3DXVECTOR3{ mRotDir * m_vCenterDir.z, 0.f, (-mRotDir) * m_vCenterDir.x };
	D3DXVec3Normalize(&vNormal, &vNormal);
	m_vCenterPos += m_vCenterDir*mTimeDelta*8.f + vNormal * mRange*0.1f;

	// 중심점으로 부모 매트릭스 만들기
	D3DXMATRIX	matScale, matRot, matTrans;
	D3DXQUATERNION qRot;

	mRotDegree += D3DXToRadian(300.f * mTimeDelta);

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	D3DXQuaternionRotationYawPitchRoll(&qRot, mRotDegree, mRotDegree, mRotDegree);
	D3DXMatrixRotationQuaternion(&matRot, &qRot);
	//TY_FixChangeTerrain_180704 Set Bullet Pos
	D3DXMatrixTranslation(&matTrans, m_vCenterPos.x, m_vCenterPos.y - 1.5f, m_vCenterPos.z);

	*m_matBulletWorld = matScale * matRot * matTrans;
}

void DNABullet::SetUpWorld_Normal()
{
	D3DXMATRIX	matScale, matRot, matTrans;
	D3DXQUATERNION qRot;

	mRotDegree += D3DXToRadian(300.f * mTimeDelta);
	m_vCenterPos += m_vCenterDir*mTimeDelta*8.f;

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	D3DXQuaternionRotationYawPitchRoll(&qRot, mRotDegree, mRotDegree, mRotDegree);
	D3DXMatrixRotationQuaternion(&matRot, &qRot);
	D3DXMatrixTranslation(&matTrans, m_vCenterPos.x, 1.2f, m_vCenterPos.z);

	*m_matBulletWorld = matScale * matRot * matTrans;
}

Bullet * DNABullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DNABullet* pInstance = new DNABullet(pDevice);
	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * DNABullet::Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag, const int dir)
{
	DNABullet*	pInstance = new DNABullet(*this);
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

DWORD DNABullet::Free(void)
{
	Bullet::Free();
	return 0;
}
