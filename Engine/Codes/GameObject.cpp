#include "GameObject.h"
#include "Component.h"

_USING(Engine);

GameObject::GameObject(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	, m_pKeyMgr(KeyMgr::GetInstance())

{
	m_pKeyMgr->AddRef();
	m_pDevice->AddRef();
}

GameObject::GameObject(const GameObject & rhs)
	:m_pDevice(rhs.m_pDevice)
	, mComponentSpace(rhs.mComponentSpace)
	, m_pKeyMgr(rhs.m_pKeyMgr)
	, mObjectName(rhs.mObjectName)
{
	for (PAIR_COMPONENT pair : mComponentSpace)
	{
		if (pair.second != nullptr)
		{
			pair.second->AddRef();
		}
	}

	m_pKeyMgr->AddRef();
	m_pDevice->AddRef();
}

HRESULT GameObject::Init()
{
	return NOERROR;
}

UPDATE GameObject::Update(const float timeDelta)
{
	return UPDATE::NORMAL;
}

UPDATE GameObject::LastUpdate(const float tTimeDelta)
{
	return UPDATE::NORMAL;
}

void GameObject::Render()
{

}

HRESULT GameObject::ReadyPrototype()
{
	return E_NOTIMPL;
}

void GameObject::Compute_ViewZ(const D3DXVECTOR3 * pPositionInWorld)
{
	D3DXMATRIX matView;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVECTOR3 vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(D3DXVECTOR3));

	D3DXVECTOR3	vDistance = *pPositionInWorld - vCamPos;

	mViewZ = D3DXVec3Length(&vDistance);
}

Component * GameObject::GetComponent(const TCHAR * pComponentTag) const
{
	return FindComponent(pComponentTag);
}

HRESULT GameObject::AddComponent(const TCHAR * pComponentTag, Component * pComponent)
{
	Component* pComponentFind = FindComponent(pComponentTag);

	if (pComponentFind != nullptr)
		return E_FAIL;

	pComponent->AddRef();

	mComponentSpace.emplace(pComponentTag, pComponent);

	return NOERROR;
}

Component * GameObject::FindComponent(const TCHAR * pComponentTag) const
{
	auto iterFind = find_if(mComponentSpace.begin(), mComponentSpace.end(), CFinder_Tag(pComponentTag));

	if (iterFind == mComponentSpace.end())
		return nullptr;

	return iterFind->second;
}

DWORD GameObject::Free()
{
	for (PAIR_COMPONENT pair : mComponentSpace)
	{
		Safe_Release(pair.second);
	//	pair.second = nullptr;
	}

	mComponentSpace.clear();

	Safe_Release(m_pKeyMgr);
	Safe_Release(m_pDevice);

	return 0;
}
