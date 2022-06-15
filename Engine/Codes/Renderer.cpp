
#include "Renderer.h"
#include "GameObject.h"

_USING(Engine)

Renderer::Renderer(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
{
}

HRESULT Renderer::Init()
{
	return NOERROR;
}

void Renderer::RenderGameObject()
{
	RenderPriority();
	RenderNonAlpha();
	RenderAlpha();
	RenderUI();
}

HRESULT Renderer::Add_RenderGroup(RENDERGROUP type, GameObject * pGameObject)
{
	if (type >= RENDER_END)
		return E_FAIL;

	mObjectSpace[type].emplace_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

void Renderer::RenderPriority()
{
	for (GameObject* pGameObject : mObjectSpace[RENDER_PRIORITY])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_PRIORITY].clear();
}

void Renderer::RenderNonAlpha()
{
	for (GameObject* pGameObject : mObjectSpace[RENDER_NONALPHA])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_NONALPHA].clear();
}

bool Compare(GameObject* pSrc, GameObject* pDest)
{
	return pSrc->GetViewZ() > pDest->GetViewZ();
}

void Renderer::RenderAlpha()
{
	mObjectSpace[RENDER_ALPHA].sort(Compare);

	for (GameObject* pGameObject : mObjectSpace[RENDER_ALPHA])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_ALPHA].clear();
}

void Renderer::RenderUI()
{
	for (GameObject* pGameObject : mObjectSpace[RENDER_UI])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_UI].clear();
}

void Renderer::ClearRenderGroup()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (GameObject* pGameObject : mObjectSpace[i])
		{
			Safe_Release(pGameObject);
		}
		mObjectSpace[i].clear();
	}
}

Renderer * Renderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Renderer* pInstance = new Renderer(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Renderer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * Renderer::Clone()
{
	AddRef();
	return this;
}

DWORD Renderer::Free(void)
{
	ClearRenderGroup();
	Component::Free();
	return 0;
}
