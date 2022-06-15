#include "ComponentMgr.h"

_USING(Engine)

_IMPLEMENT_SINGLETON(ComponentMgr)

ComponentMgr::ComponentMgr()
{

}

HRESULT ComponentMgr::ReserveContainer(const WORD size)
{
	if (m_pPrototypeSpace != nullptr)
		return E_FAIL;

	m_pPrototypeSpace = new MAP_PROTOTYPE[size];

	mReserveSize = size;
	m_bReserved = true;

	return NOERROR;
}

HRESULT ComponentMgr::AddPrototype(const WORD sceneIdx, const TCHAR * pComponentTag, Component * pComponent)
{
	if (m_bReserved == false || m_pPrototypeSpace == nullptr)
		return E_FAIL;

	Component* pComponentFind = FindComponent(sceneIdx, pComponentTag);

	if (pComponentFind != nullptr)
		return E_FAIL;

	m_pPrototypeSpace[sceneIdx].emplace(pComponentTag, pComponent);

	return NOERROR;
}

Component * ComponentMgr::Clone(const WORD sceneIdx, const TCHAR * pComponentTag)
{
	if (m_bReserved == false || m_pPrototypeSpace == nullptr)
		return nullptr;

	Component* pComponentFind = FindComponent(sceneIdx, pComponentTag);

	if (pComponentFind == nullptr)
		return nullptr;

	return pComponentFind->Clone();
}

Component * ComponentMgr::FindComponent(const WORD sceneIdx, const TCHAR * pComponentTag)
{
	auto iterFind = find_if(m_pPrototypeSpace[sceneIdx].begin(), m_pPrototypeSpace[sceneIdx].end(), CFinder_Tag(pComponentTag));

	if (iterFind == m_pPrototypeSpace[sceneIdx].end())
		return nullptr;

	return iterFind->second;
}

DWORD ComponentMgr::Free(void)
{
	for (size_t i = 0; i < mReserveSize; ++i)
	{
		for (PAIR_PROTOTYPE pair : m_pPrototypeSpace[i])
		{
			Safe_Release(pair.second);
		}
		m_pPrototypeSpace[i].clear();
	}

	Safe_Delete_Array(m_pPrototypeSpace);

	return 0;
}
