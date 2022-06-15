#ifndef COMPONENT_H__
#define COMPONENT_H__

#include "Base.h"

_BEGIN(Engine);

class _ENGINE_DLL Component abstract : public Base
{
protected:
	explicit Component(LPDIRECT3DDEVICE9 pDevice);
	explicit Component(const Component& rhs);
	virtual ~Component() = default;

public:
	virtual Component* Clone() = 0;
	virtual DWORD Free();

protected:
	bool m_bClone = false;

protected:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

};

_END

#endif

