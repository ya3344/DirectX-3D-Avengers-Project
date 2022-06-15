#ifndef PLANET_MGR_H
#define PLANET_MGR_H

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class  PlanetMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(PlanetMgr);

private:
	explicit PlanetMgr();
	virtual ~PlanetMgr() = default;

public:
	HRESULT ReadyPlanet();

public:
	void Set_PlanetInfo(const TCHAR* pName, const PLANET_INFO& PlanetInfo);
	void Set_CenterPos();

public:

	// Base을(를) 통해 상속됨
	virtual DWORD Free() override;

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

_END

#endif
