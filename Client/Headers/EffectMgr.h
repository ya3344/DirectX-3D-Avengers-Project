#ifndef EFFECT_MGR_H
#define EFFECT_MGR_H

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class  EffectMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(EffectMgr);

private:
	explicit EffectMgr();
	virtual ~EffectMgr() = default;

public:
	HRESULT ReadyEffect();

public:
	void EffectOperation(const TCHAR* pName, const EFFECT_INFO& effectInfo);

public:

	// Base을(를) 통해 상속됨
	virtual DWORD Free() override;

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

_END

#endif
