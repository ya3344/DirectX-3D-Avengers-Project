#include "stdafx.h"
#include "Sphere.h"
#include "ComponentMgr.h"

_USING(Client)

Sphere::Sphere(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{
}

Sphere::Sphere(const Sphere & rhs)
	: GameObject(rhs)
{
}

HRESULT Sphere::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	if (m_pDevice == nullptr)
		return E_FAIL;

	mRadius = 1.f;
	m_bDrawSphere = false;

	D3DXCreateSphere(m_pDevice, mRadius, 10, 10, &m_pSphereMesh, NULL);

	return NOERROR;
}

UPDATE Sphere::Update(const float timeDelta)
{
	if (m_pKeyMgr->KeyPressing(DIK_M))
	{
		m_bDrawSphere = !m_bDrawSphere;
	}

	return UPDATE::NORMAL;
}

UPDATE Sphere::LastUpdate(const float timeDelta)
{
	return UPDATE::NORMAL;
}

void Sphere::Render()
{
	if (m_pSphereMesh == nullptr
		|| m_pTargeInfo == nullptr
		|| m_pTransform == nullptr)
	{
		_MSG_BOX("Render Sphere Failed");
		return;
	}

	m_vPos = *m_pTargeInfo->GetInfo(Engine::Transform::INFO_POSITION);
	// TY_Bullet, Sphere Fix_180702_ Change Y value 
	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ m_vPos.x, 2.f, m_vPos.z});
	m_pTransform->SetUp_World();

#ifdef _DEBUG
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_bDrawSphere)
	{
		m_pSphereMesh->DrawSubset(0);
	}

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#endif

}

HRESULT Sphere::Ready_GameObjectPrototype()
{
	Init();
	return NOERROR;
}

HRESULT Sphere::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	return NOERROR;
}

void Sphere::SetRadius(const float radius)
{
	Engine::Safe_Release(m_pSphereMesh);
	D3DXCreateSphere(m_pDevice, radius, 10, 10, &m_pSphereMesh, NULL);
	mRadius = radius;
}

D3DXVECTOR3 Sphere::GetCenterPos() const
{
	// TY_Bullet, Sphere Fix_180702_ Change Y value 
	D3DXVECTOR3 vPos = D3DXVECTOR3{ m_vPos.x, 2.f, m_vPos.z };

	return vPos;
}


Sphere * Sphere::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Sphere* pInstance = new Sphere(pDevice);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
	{
		_MSG_BOX("Sphere Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Sphere::Clone()
{
	AddRef();
	return this;
}

DWORD Sphere::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pSphereMesh);

	GameObject::Free();

	return 0;
}
