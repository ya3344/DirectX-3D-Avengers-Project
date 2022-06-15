#include "stdafx.h"
#include "SkyBox.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Camera.h"

_USING(Client)


SkyBox::SkyBox(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{
}

SkyBox::SkyBox(const SkyBox & rhs)
	: Engine::GameObject(rhs)
{
}

HRESULT SkyBox::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	return NOERROR;
}

UPDATE SkyBox::Update(const float timeDelta)
{

	return Engine::GameObject::Update(timeDelta);
}

UPDATE SkyBox::LastUpdate(const float timeDelta)
{
	//DH_GetMatrix_View_180613 BackUp
	/*D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	memcpy(&m_vTargetPos, &matView.m[3][0], sizeof(D3DXVECTOR3));*/

	Engine::ObjectMgr* pObjectMgr = nullptr;
	Engine::Camera* pCamera = (Engine::Camera*)pObjectMgr->GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Camera", 0);

	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	m_vTargetPos = pCamera->GetViewInfo().vEye;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &m_vTargetPos);

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_PRIORITY, this)))
		return UPDATE::FAIL;



	return Engine::GameObject::LastUpdate(timeDelta);
}

void SkyBox::Render()
{

	if (m_pBuffer == nullptr)
		return;

	m_pTexture->SetUp_Device(0);
	m_pTransform->SetUp_World();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


}

HRESULT SkyBox::ReadyPrototype()
{



	return NOERROR;
}

HRESULT SkyBox::ReadyComponent(void)
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
	pComponent = m_pBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Texture_SkyBox");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_SkyBox", pComponent)))
		return E_FAIL;

	return NO_ERROR;
}

SkyBox * SkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	SkyBox* pInstance = new SkyBox(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("SkyBox Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * SkyBox::Clone()
{
	SkyBox*	pInstance = new SkyBox(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("SkyBox Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD SkyBox::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return DWORD();
}
