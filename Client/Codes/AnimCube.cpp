#include "stdafx.h"
#include "AnimCube.h"
#include "ComponentMgr.h"

_USING(Client)

AnimCube::AnimCube(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{

}

AnimCube::AnimCube(const AnimCube & rhs)
	: Engine::GameObject(rhs)
	, m_pTransform(rhs.m_pTransform)
	, m_pBuffer(rhs.m_pBuffer)
	, m_pTexture(rhs.m_pTexture)
	, m_pAnimNode(rhs.m_pAnimNode)
	, mTextureNum(rhs.mTextureNum)
{
	m_pTransform->AddRef();
	m_pBuffer->AddRef();
	m_pTexture->AddRef();
	m_pAnimNode->AddRef();
}

HRESULT AnimCube::Init()
{
	if (m_bAlreadyComponent == false)
	{
		if (FAILED(ReadyComponent(L"", 0)))
			return E_FAIL;
	}

	//m_pTransform->SetRotation(D3DXToRadian(45.0f));
	//m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(rand()%10 + 5.f, rand() % 10 + 5.f, rand() % 10 + 5.f));

	return NOERROR;
}

UPDATE AnimCube::Update(const float timeDelta)
{
	return Engine::GameObject::Update(timeDelta);
}

UPDATE AnimCube::LastUpdate(const float timeDelta)
{
	return Engine::GameObject::LastUpdate(timeDelta);
}

void AnimCube::Render()
{
	if (m_pAnimNode != nullptr)
	{
		if (m_pAnimNode->GetName() == L"Waist" || m_bShiledRender == true)
			return;
	}

	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();
}

HRESULT AnimCube::Ready_GameObjectPrototype(const ANIM_CUBE_INFO& animCube_Info)
{
	m_bAlreadyComponent = animCube_Info.bAlreadyComponent;

	if (m_bAlreadyComponent)
	{
		if (FAILED(ReadyComponent(animCube_Info.pComTextureTag, animCube_Info.comSceneIdx)))
			return E_FAIL;

		m_pAnimNode->SetName(animCube_Info.name);
		m_pAnimNode->Set_ParentName(animCube_Info.parentName);
		m_pAnimNode->SetMatrixWorld(m_pTransform->GetWolrdPointer());
	}

	return NOERROR;
}

HRESULT AnimCube::ReadyComponent(const TCHAR* pComTextureTag, const WORD comSceneIdx)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(comSceneIdx, pComTextureTag);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(pComTextureTag, pComponent)))
		return E_FAIL;

	// For.Com_Animation
	pComponent = m_pAnimNode = (Engine::AnimNode*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Animation");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Animation", pComponent)))
		return E_FAIL;

	return NO_ERROR;
}

AnimCube * AnimCube::Create(LPDIRECT3DDEVICE9 pDevice, const ANIM_CUBE_INFO& animCube_Info)
{
	AnimCube* pInstance = new AnimCube(pDevice);
	pInstance->setName(animCube_Info.name);
	pInstance->SetTextureNum(animCube_Info.textureNum);

	if (FAILED(pInstance->Ready_GameObjectPrototype(animCube_Info)))
	{
		_MSG_BOX("AnimCube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * AnimCube::Clone()
{
	AnimCube*	pInstance = new AnimCube(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("AnimCube Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD AnimCube::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);
	Engine::Safe_Release(m_pAnimNode);

	GameObject::Free();

	return DWORD();
}
