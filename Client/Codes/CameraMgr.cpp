#include "stdafx.h"
#include "ObjectMgr.h"
#include "CameraMgr.h"
#include "Layer.h"
#include "Camera.h"
#include "CameraAction.h"
#include "CameraTps.h"

#include "Include.h"

_USING(Client)

_IMPLEMENT_SINGLETON(CameraMgr)
CameraMgr::CameraMgr()
{
}

HRESULT CameraMgr::Init()
{
	m_pKeyMgr = Engine::KeyMgr::GetInstance();
	m_pKeyMgr->AddRef();

	if (m_pKeyMgr == nullptr)
		return E_FAIL;

	return NOERROR;
}

HRESULT CameraMgr::Update_Camera()
{
	if (m_pNewCamera_Tag != nullptr && lstrcmp(m_pNewCamera_Tag, m_pCurCamera_Tag))
	{
		if (FAILED(Ready_LayerCamera(m_pNewCamera_Tag)))
			return E_FAIL;
	}


	// Camera Select key Checker For Camera Select
	if (m_pKeyMgr->KeyUp(DIK_1))
	{
		if (FAILED(Ready_LayerCamera(L"Prototype_GameObject_CameraAction")))
			return E_FAIL;
	}
	else if (m_pKeyMgr->KeyUp(DIK_2))
	{
		if (FAILED(Ready_LayerCamera(L"Prototype_GameObject_CameraFree")))
			return E_FAIL;
	}
	else if (m_pKeyMgr->KeyUp(DIK_3))
	{
		if (FAILED(Ready_LayerCamera(L"Prototype_GameObject_CameraTps")))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CameraMgr::ChangeCamera(TCHAR* pCameraTag)
{
	if (pCameraTag == nullptr)
	{
		_MSG_BOX("ChangeCamera Failed");
		return E_FAIL;
	}

	m_pNewCamera_Tag = pCameraTag;

	return NOERROR;
}

HRESULT CameraMgr::Ready_LayerCamera(TCHAR * pCameraTag)
{
	if (pCameraTag == nullptr)
		return E_FAIL;

	if (m_pCurCamera_Tag != nullptr && lstrcmp(m_pCurCamera_Tag, pCameraTag) == 0)
		return NOERROR;

	//TY_FixChangeTerrain_180704
	m_pNewCamera_Tag = pCameraTag;
	m_pCurCamera_Tag = pCameraTag;
	// camera Layer 싹다 비우는 함수 호출 후 add Layer
	Engine::ObjectMgr::GetInstance()->ClearLayer(SCENE_STAGE, L"Layer_Camera");

	//TY_Ready_LayerCamera_180613 Add CameraAction To Layer
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STAGE, pCameraTag, SCENE_STAGE, L"Layer_Camera")))
		return E_FAIL;

	return NOERROR;
}

void CameraMgr::ShakeCamera(const float shakeTime)
{
	Engine::Camera* pCamera = nullptr;

	pCamera = (Engine::Camera*)Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_STAGE, L"Layer_Camera")->GetGameObject();

	if (pCamera == nullptr)
	{
		_MSG_BOX("Shake Camera Failed");
		return;
	}

	pCamera->SetCameraShake(shakeTime);
}

// TY_ChangeStage_180703 _Function To Init ActionCam
void CameraMgr::InitActionCam(TCHAR * pPathName)
{
	if (m_pCurCamera_Tag == nullptr || lstrcmp(m_pCurCamera_Tag, L"Prototype_GameObject_CameraAction") == 0)
		return;

	if (FAILED(Ready_LayerCamera(L"Prototype_GameObject_CameraAction")))
	{
		_MSG_BOX("CameraMgr InitAction Failed");
	}
	m_pNewCamera_Tag = L"Prototype_GameObject_CameraAction";

	CameraAction* pCamera = nullptr;

	pCamera = (CameraAction*)Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_STAGE, L"Layer_Camera")->GetGameObject();
	pCamera->SetPathData(pPathName);
}

//TY_TPS ZoomIn_180705_ Functino To SetZoom
void CameraMgr::ZoomCamera_TPS(const D3DXVECTOR3 & vDest)
{
	if (m_pCurCamera_Tag == nullptr || lstrcmp(m_pCurCamera_Tag, L"Prototype_GameObject_CameraTps") != 0)
		return;

	CameraTps* pCamera = nullptr;

	pCamera = (CameraTps*)Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_STAGE, L"Layer_Camera")->GetGameObject();
	pCamera->SetZoom(vDest);
}

DWORD CameraMgr::Free()
{
	Engine::Safe_Release(m_pKeyMgr);
	return 0;
}
