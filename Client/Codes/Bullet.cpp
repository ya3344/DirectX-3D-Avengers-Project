#include "stdafx.h"
#include "..\Headers\Bullet.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Sphere.h"

_USING(Client)

Bullet::Bullet(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
	, m_pSoundMgr(Engine::SoundMgr::GetInstance())
{
	m_pSoundMgr->AddRef();
}
Bullet::Bullet(const Bullet & rhc)
	: Engine::GameObject(rhc)
	, m_pSoundMgr(Engine::SoundMgr::GetInstance())
{
	m_pSoundMgr->AddRef();
}

HRESULT Bullet::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;
	// For.Com_Renderer

	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_BULLET, L"Component_Texture_Bullet");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_Bullet", pComponent)))
		return E_FAIL;

	//TY_Sphere_180618 Add Sphere Object
	Engine::GameObject* pGameObject = m_pSphere = Sphere::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	m_pSphere->SetGameObject(m_pTransform);
	m_pSphere->SetRadius(0.1f);

	return NO_ERROR;
}

HRESULT Bullet::ReadyPrototype()
{
	return NO_ERROR;
}


const D3DXVECTOR3 * Bullet::GetDir()
{
	return m_pTransform->GetInfo(Engine::Transform::INFO_LOOK);
}

Engine::GameObject * Bullet::Clone()
{
	_MSG_BOX("Bullet Clone Failed");

	return nullptr;
}

DWORD Bullet::Free(void)
{
	Engine::Safe_Release(m_pSphere);

	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pSoundMgr);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return DWORD();
}
