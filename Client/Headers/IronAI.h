#ifndef IRON_AI_H__
#define IRON_AI_H__

#include "BossAI.h"

_BEGIN(Client)

class IronAI final : public BossAI
{
private:
	explicit IronAI(LPDIRECT3DDEVICE9 pDevice);
	explicit IronAI(const IronAI& rhs);
	virtual ~IronAI() = default;

public:
	// BossAI을(를) 통해 상속됨
	virtual void AnimState_Change(const float timeDelta) override;
	virtual void AnimUpdate(const float timeDelta) override;
	virtual void SelectAttackMode() override;
	virtual void DestroyPart() override;

public:
	static IronAI* Create(LPDIRECT3DDEVICE9 pDevice, SUPER_CUBE_INFO* pSuperCubeInfo);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

	//TY_FixCamera_180704_ Play Action Cam
private:
	bool m_bDead_ActionCam = true;
};

_END
#endif

