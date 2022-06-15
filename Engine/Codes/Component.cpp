#include "Component.h"

_USING(Engine);


Component::Component(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

Component::Component(const Component & rhs)
	:m_pDevice(rhs.m_pDevice)
{
	m_pDevice->AddRef();
	m_bClone = true;
}


DWORD Component::Free(void)
{
	Safe_Release(m_pDevice);
	return 0;
}
