#include "stdafx.h"
#include "UI.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
_USING(Client)

UI::UI(LPDIRECT3DDEVICE9 pDevice)
	:Engine::GameObject(pDevice)
	, m_pTransform(nullptr)
	, m_pRenderer(nullptr)
	, m_pSoundMgr(Engine::SoundMgr::GetInstance())
{
	m_pSoundMgr->AddRef();
}

UI::UI(const UI& rhs)
	: Engine::GameObject(rhs)
	, mUIInfo(rhs.mUIInfo)
	, mTextureNum(rhs.mTextureNum)
	, m_pSoundMgr(rhs.m_pSoundMgr)
{
}

HRESULT UI::Init()
{
	return NOERROR;
}

UPDATE UI::Update(const float timeDelta)
{
	return Engine::GameObject::Update(timeDelta);
}

UPDATE UI::LastUpdate(const float timeDelta)
{
	if (nullptr == m_pRenderer)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_UI, this)))
		return UPDATE::FAIL;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void UI::Render(const bool bExit)
{
	if (nullptr == m_pBuffer)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (bExit)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	m_pTexture->SetUp_Device(mTextureNum);
	m_pBuffer->Render_Buffer(m_pTransform->GetWolrdPointer());

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT UI::ReadyPrototype()
{
	return NOERROR;
}

HRESULT UI::ReadyComponent(void)
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
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBuffer = (Engine::UITex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_UITexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_UITexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STAGE, L"Component_Texture_UI");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_UI", pComponent)))
		return E_FAIL;


	return NOERROR;
}

DWORD UI::Free()
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);
	Engine::Safe_Release(m_pSoundMgr);


	GameObject::Free();

	return 0;
}
