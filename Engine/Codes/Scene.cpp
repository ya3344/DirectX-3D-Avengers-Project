#include "Scene.h"
#include "ObjectMgr.h"

_USING(Engine)

Scene::Scene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pObjectMgr(ObjectMgr::GetInstance())
{
	m_pDevice->AddRef();
	m_pObjectMgr->AddRef();
}

HRESULT Scene::Init()
{
	return NOERROR;
}

UPDATE Scene::Update(const float timeDelta)
{
	if (m_pObjectMgr == nullptr)
		return UPDATE::FAIL;

	 UPDATE exitCode = UPDATE::FAIL;

	 exitCode = m_pObjectMgr->Update_GameObject(timeDelta);

	if (exitCode == UPDATE::FAIL)
		return exitCode;

	return exitCode;
}

UPDATE Scene::LastUpdate(const float timeDelta)
{
	if (m_pObjectMgr == nullptr)
		return UPDATE::FAIL;

	UPDATE exitCode = UPDATE::FAIL;

	exitCode = m_pObjectMgr->LastUpdate_GameObject(timeDelta);

	if (exitCode == UPDATE::FAIL)
		return exitCode;

	return exitCode;
}

void Scene::Render()
{
}

HRESULT Scene::Add_GameObjectPrototype(const TCHAR * pProtoTag, const size_t sceneIdx, GameObject * pGameObject)
{
	if (m_pObjectMgr == nullptr)
		return E_FAIL;

	if (FAILED(m_pObjectMgr->Add_GameObjectPrototype(pProtoTag, sceneIdx, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Scene::Add_GameObjectToLayer(const size_t sceneIdx_Proto, const TCHAR * pProtoTag, const size_t sceneIdx, const TCHAR * pLayerTag)
{
	if (m_pObjectMgr == nullptr)
		return E_FAIL;

	if (FAILED(m_pObjectMgr->Add_GameObjectToLayer(sceneIdx_Proto, pProtoTag, sceneIdx, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

void Scene::ClearGameObject(const size_t sceneIdx)
{
	if (m_pObjectMgr == nullptr)
		return;

	m_pObjectMgr->Clear_GameObject(sceneIdx);
}

//TY_ClearLayer_180614 Add Function To Call Layer Function of ObjectMgr Class
HRESULT Scene::ClearLayer(const size_t sceneIdx, const TCHAR * pLayerTag)
{
	if (m_pObjectMgr == nullptr)
		return E_FAIL;

	if (FAILED(m_pObjectMgr->ClearLayer(sceneIdx, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

DWORD Scene::Free()
{
	Safe_Release(m_pObjectMgr);
	Safe_Release(m_pDevice);

	return 0;
}
