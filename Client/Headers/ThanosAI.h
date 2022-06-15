#ifndef THANOS_AI_H__
#define THANOS_AI_H__

#include "BossAI.h"

_BEGIN(Client)

class ThanosAI final : public BossAI
{
public:
	explicit ThanosAI(LPDIRECT3DDEVICE9 pDevice);
	explicit ThanosAI(const ThanosAI& rhs);
	virtual ~ThanosAI() = default;

public:
	// BossAI을(를) 통해 상속됨
	virtual void AnimState_Change(const float timeDelta) override;
	virtual void AnimUpdate(const float timeDelta) override;
	virtual void SelectAttackMode() override;
	virtual void DestroyPart() override;

public:
	static ThanosAI* Create(LPDIRECT3DDEVICE9 pDevice, SUPER_CUBE_INFO* pSuperCubeInfo);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};

_END

#endif

