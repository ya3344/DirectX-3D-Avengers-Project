#include "stdafx.h"
#include "EffectMgr.h"
#include "Effect.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"


_USING(Client)
_IMPLEMENT_SINGLETON(EffectMgr)

EffectMgr::EffectMgr()
	:m_pDevice(Engine::GraphicDevice::GetInstance()->GetDevice())
{
	m_pDevice->AddRef();
}

HRESULT EffectMgr::ReadyEffect()
{
	Engine::GameObject* pGameObject = nullptr;

	{

		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_Sword", 11, 35.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Boss_Sword_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Boss_Sword_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_Boss_Sword_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}


	{
		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_Gun", 11, 25.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Boss_Gun_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Boss_Gun_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_Boss_Gun_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}

	{
		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_Explosion", 9, 15.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Explosion_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Explosion_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_Explosion_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}

	{
		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_Explosion2", 90, 35.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Explosion2_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Explosion2_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_Explosion2_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}


	{
		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_Dust", 54, 25.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Dust_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Dust_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_Dust_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}

	{
		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_EggHit", 37, 33.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"EggHit_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_EggHit_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_EggHit_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}

	{
		pGameObject = Effect::Create(m_pDevice, EFFECT_INIT_INFO(L"Component_Texture_MagicHit", 4, 15.f));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Player_Hit_Effect");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Hit_Effect", SCENE_PARTICLE, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PARTICLE, L"Prototype_Hit_Effect", SCENE_PARTICLE, L"Layer_Effect")))
			return E_FAIL;
	}

	return NO_ERROR;
}

void EffectMgr::EffectOperation(const TCHAR* pName, const EFFECT_INFO& effectInfo)
{
	Effect* pEffectObj = (Effect*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PARTICLE, L"Layer_Effect", pName);
	if (pEffectObj == nullptr)
	{
		_MSG_BOX("Effect Get Failed!!");
		return;
	}

	pEffectObj->EffectOperation(effectInfo);
		
}

DWORD EffectMgr::Free()
{
	Engine::Safe_Release(m_pDevice);
	return 0;
}


