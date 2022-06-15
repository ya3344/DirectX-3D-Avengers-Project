#ifndef BUCK_BULLET_H__ 
#define BUCK_BULLET_H__

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class BuckBullet final : public Bullet {

private:
	explicit BuckBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit BuckBullet(const BuckBullet& rhs);
	virtual ~BuckBullet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

private:
	HRESULT ReadyBulletType(const BULLET_INFO& bulletInfo);

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag);
	virtual DWORD Free(void) override;
};
_END


#endif