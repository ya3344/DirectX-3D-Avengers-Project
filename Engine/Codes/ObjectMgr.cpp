#include "ObjectMgr.h"
#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

_USING(Engine);

_IMPLEMENT_SINGLETON(ObjectMgr)

ObjectMgr::ObjectMgr()
{
}

HRESULT ObjectMgr::ReserveContainer(const WORD size)
{
	if (m_pProtoTypeSpace != nullptr ||
		m_pLayerSpace != nullptr)
	{
		return E_FAIL;
	}

	m_pProtoTypeSpace = new PROTOTYPE_MAP[size];
	m_pLayerSpace = new LAYER_MAP[size];

	mReserveSize = size;
	m_bReserved = true;

	return NOERROR;
}

HRESULT ObjectMgr::Add_GameObjectPrototype(const TCHAR * pProtoTag, const size_t sceneIdx, GameObject * pGameObject)
{
	if (m_pProtoTypeSpace == nullptr || mReserveSize <= sceneIdx)
		return E_FAIL;

	GameObject* pProtoType = FindPrototype(sceneIdx, pProtoTag);
	if (pProtoType != nullptr)
		return E_FAIL;

	m_pProtoTypeSpace[sceneIdx].emplace(pProtoTag, pGameObject);

	return NOERROR;
}

HRESULT ObjectMgr::Add_GameObjectToLayer(const size_t sceneIdx_Proto, const TCHAR * pProtoTag, const size_t sceneIdx, const TCHAR * pLayerTag)
{
	if (m_pLayerSpace == nullptr || mReserveSize <= sceneIdx)
		return E_FAIL;

	GameObject* pPrototype = FindPrototype(sceneIdx_Proto, pProtoTag);
	if (pPrototype == nullptr)
		return E_FAIL;

	Layer* pLayer = FindLayer(sceneIdx, pLayerTag);

	if (pLayer == nullptr)
	{
		Layer* pLayer = Layer::Create();
		if (pLayer == nullptr)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pPrototype->Clone())))
			return E_FAIL;

		m_pLayerSpace[sceneIdx].emplace(pLayerTag, pLayer);
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pPrototype->Clone())))
			return E_FAIL;
	}

	return NOERROR;
}

UPDATE ObjectMgr::Update_GameObject(const float timeDelta)
{
	UPDATE exitCode = UPDATE::FAIL;

	for (size_t i = 0; i < mReserveSize; ++i)
	{
		for (LAYER_PAIR pair : m_pLayerSpace[i])
		{
			if (pair.second == nullptr)
				continue;

			exitCode = pair.second->Update(timeDelta);

			if (exitCode == UPDATE::FAIL)
				return exitCode;
		}
	}

	return exitCode;
}

UPDATE ObjectMgr::LastUpdate_GameObject(const float timeDelta)
{
	UPDATE exitCode = UPDATE::FAIL;

	for (size_t i = 0; i < mReserveSize; ++i)
	{
		for (LAYER_PAIR pair : m_pLayerSpace[i])
		{
			if (pair.second == nullptr)
				continue;

			exitCode = pair.second->LastUpdate(timeDelta);

			if (exitCode == UPDATE::FAIL)
				return exitCode;
		}
	}

	return exitCode;
}

Component * ObjectMgr::GetComponent(const size_t sceneIdx, const TCHAR * pLayerTag, const size_t index, const TCHAR * pComponentTag)
{
	Layer* pLayer = FindLayer(sceneIdx, pLayerTag);

	if (pLayer == nullptr)
		return nullptr;

	return pLayer->GetComponent(index, pComponentTag);
}

GameObject * ObjectMgr::GetGameObject(const size_t sceneIdx, const TCHAR * pLayerTag, const TCHAR* pGameObjectTag)
{
	Layer* pLayer = FindLayer(sceneIdx, pLayerTag);

	GameObject* pGameObject = pLayer->GetGameObject(pGameObjectTag);
	if (pGameObject == nullptr)
		return nullptr;

	return pGameObject;
}

GameObject * ObjectMgr::FindPrototype(const size_t sceneIdx, const TCHAR * pProtoTag)
{
	if (m_pProtoTypeSpace == nullptr || mReserveSize <= sceneIdx)
		return nullptr;

	auto iterFind = find_if(m_pProtoTypeSpace[sceneIdx].begin(), m_pProtoTypeSpace[sceneIdx].end(), CFinder_Tag(pProtoTag));

	if (iterFind == m_pProtoTypeSpace[sceneIdx].end())
		return nullptr;

	return iterFind->second;
}


Layer * ObjectMgr::FindLayer(const size_t sceneIdx, const TCHAR * pLayerTag)
{
	if (m_pLayerSpace == nullptr || mReserveSize <= sceneIdx)
		return nullptr;

	auto iterFind = find_if(m_pLayerSpace[sceneIdx].begin(), m_pLayerSpace[sceneIdx].end(), CFinder_Tag(pLayerTag));

	if (iterFind == m_pLayerSpace[sceneIdx].end())
		return nullptr;

	return iterFind->second;
}

//TY_ClearLayer_180614 Add Function To Call Layer Clear Function of Layer Class
HRESULT ObjectMgr::ClearLayer(const size_t sceneIdx, const TCHAR * pLayerTag)
{
	if (m_pLayerSpace == nullptr || mReserveSize <= sceneIdx)
		return E_FAIL;

	auto iterFind = find_if(m_pLayerSpace[sceneIdx].begin(), m_pLayerSpace[sceneIdx].end(), CFinder_Tag(pLayerTag));

	if (iterFind == m_pLayerSpace[sceneIdx].end())
		return E_FAIL;

	(*iterFind).second->ClearLayer();
	return NOERROR;
}

DWORD ObjectMgr::Free()
{
	for (size_t i = 0; i < mReserveSize; i++)
	{
		for (LAYER_PAIR pair : m_pLayerSpace[i])
		{
			Safe_Release(pair.second);
		}
			
		m_pLayerSpace[i].clear();
	}

	Safe_Delete_Array(m_pLayerSpace);

	for (size_t i = 0; i < mReserveSize; i++)
	{
		for (PROTOTYPE_PAIR pair : m_pProtoTypeSpace[i])
		{
			Safe_Release(pair.second);
		}
			
		m_pProtoTypeSpace[i].clear();
	}
	Safe_Delete_Array(m_pProtoTypeSpace);


	return DWORD();
}

void ObjectMgr::Clear_GameObject(const size_t sceneIdx)
{
	if (mReserveSize <= sceneIdx)
		return;

	for (LAYER_PAIR pair : m_pLayerSpace[sceneIdx])
	{
		//Safe_DoubleRelease(&pair.second);
		Safe_Release(pair.second);
	}


	m_pLayerSpace[sceneIdx].clear();


	for (PROTOTYPE_PAIR pair : m_pProtoTypeSpace[sceneIdx])
	{
		//Safe_DoubleRelease(&pair.second);
		Safe_Release(pair.second);
	}
	
	m_pProtoTypeSpace[sceneIdx].clear();
}
