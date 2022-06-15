#include "stdafx.h"
#include "Logo.h"
#include "BackLogo.h"
#include "Stage.h"
#include "Management.h"

_USING(Client)

Logo::Logo(LPDIRECT3DDEVICE9 pDevice) : Scene(pDevice)
{
}

HRESULT Logo::Init()
{
	// 현재 씬에 필요한 원형객체들을 생성하자.
	if (FAILED(Ready_GameObjectPrototype()))
		return E_FAIL;

	// 로고 씬에서 필요한 객체와 컴포넌트들을 생성하자.
	if (FAILED(Ready_LayerBackGround(L"Layer_BackGround")))
		return E_FAIL;

	return NOERROR;
}

UPDATE Logo::Update(const float timeDelta)
{
	return Scene::Update(timeDelta);
}

UPDATE Logo::LastUpdate(const float timeDelta)
{
	Scene::LastUpdate(timeDelta);
	
	if (g_bLoading)
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			Scene* pNewScene = Stage::Create(m_pDevice);
			if (pNewScene == nullptr)
				return UPDATE::FAIL;

			if (FAILED(Engine::Management::GetInstance()->SetUp_CurrentScene(pNewScene)))
				return UPDATE::FAIL;

			Engine::Safe_Release(pNewScene);

			return UPDATE::NORMAL;
		}
	}


	return UPDATE::NORMAL;
}

void Logo::Render(void)
{
}

HRESULT Logo::Ready_GameObjectPrototype()
{
	Engine::GameObject* pGameObject = nullptr;

	// For.Prototype_GameObject_BackLogo
	pGameObject = BackLogo::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_BackLogo", SCENE::SCENE_LOGO, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Logo::ReadyComponent()
{
	return E_NOTIMPL;
}

HRESULT Logo::Ready_LayerBackGround(const TCHAR * pLayerTag)
{
	// 추가한다. (추가할려고했던 레이어가 있냐없냐, 없으면 생성)
	if (FAILED(Add_GameObjectToLayer(SCENE::SCENE_LOGO, L"Prototype_GameObject_BackLogo", SCENE::SCENE_LOGO, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

Logo * Logo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Logo* pInstance = new Logo(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Logo::Free()
{
	Engine::Scene::ClearGameObject(SCENE_LOGO);
	Engine::Scene::Free();
	return 0;
}
