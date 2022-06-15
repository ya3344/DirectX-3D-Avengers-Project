#include "stdafx.h"
#include "PlanetMgr.h"
#include "Planet.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "Layer.h"


_USING(Client)
_IMPLEMENT_SINGLETON(PlanetMgr)

PlanetMgr::PlanetMgr()
	:m_pDevice(Engine::GraphicDevice::GetInstance()->GetDevice())
{
	m_pDevice->AddRef();
}

HRESULT PlanetMgr::ReadyPlanet()
{
	Engine::GameObject* pGameObject = nullptr;

	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(0, 200.f, 15.f, D3DXVECTOR3(30.f, 3.f, 15.f), D3DXVECTOR3(2.5f, 2.5f, 2.5f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Earth");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Earth", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Earth", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}

	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(1, 260.f, -65.f, D3DXVECTOR3(30.f, 2.f, 15.f), D3DXVECTOR3(3.f, 3.f, 3.f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Magma");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Magma", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Magma", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}

	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(2, -180.f, 65.f, D3DXVECTOR3(25.f, 1.5f, 30.f), D3DXVECTOR3(3.3f, 3.3f, 3.3f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Mars");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Mars", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Mars", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}

	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(3, 80.f, -45.f, D3DXVECTOR3(10.f, 1.5f, 25.f), D3DXVECTOR3(2.f, 2.f, 2.f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Rand");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Rand", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Rand", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}


	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(1, -200.f, 25.f, D3DXVECTOR3(30.f, 3.5f, 25.f), D3DXVECTOR3(3.5f, 3.5f, 3.5f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Magma2");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Magma2", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Magma2", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}



	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(0, -200.f, -35.f, D3DXVECTOR3(30.f, 2.5f, 15.f), D3DXVECTOR3(2.5f, 2.5f, 2.5f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Earth2");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Earth2", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Earth2", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}


	{
		pGameObject = Planet::Create(m_pDevice, PLANET_INFO(3, 80.f, 45.f, D3DXVECTOR3(20.f, 1.5f, 25.f), D3DXVECTOR3(2.f, 2.f, 2.f)));
		if (nullptr == pGameObject)
			return E_FAIL;
		pGameObject->setName(L"Planet_Rand2");

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Planet_Rand2", SCENE_PLANENT, pGameObject)))
			return E_FAIL;

		// For.Effect
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_PLANENT, L"Prototype_Planet_Rand2", SCENE_PLANENT, L"Layer_Planet")))
			return E_FAIL;
	}
	

	return NO_ERROR;
}

void PlanetMgr::Set_PlanetInfo(const TCHAR* pName, const PLANET_INFO& PlanetInfo)
{
	Planet* pPlanetObj = (Planet*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PLANENT, L"Layer_Planet", pName);
	if (pPlanetObj == nullptr)
	{
		_MSG_BOX("Planet Get Failed!!");
		return;
	}

	pPlanetObj->Set_PlanetInfo(PlanetInfo);
	
}

void PlanetMgr::Set_CenterPos()
{
	 Engine::Layer* pLayer = Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_PLANENT, L"Layer_Planet");
	 
	 auto iterBegin = pLayer->Get_GameObjectSpace()->begin();
	 auto iterEnd = pLayer->Get_GameObjectSpace()->end();

	 for (; iterBegin != iterEnd; ++iterBegin)
	 {
		 Planet* pPlanet = (Planet*)*iterBegin;

		 pPlanet->Set_CenterPos();
	 }
}

DWORD PlanetMgr::Free()
{
	Engine::Safe_Release(m_pDevice);
	return 0;
}


