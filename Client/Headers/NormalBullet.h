#ifndef NORMAL_BULLET_H__ 
#define NORMAL_BULLET_H__

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class NormalBullet final : public Bullet {

private:
	explicit NormalBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit NormalBullet(const NormalBullet& rhs);
	virtual ~NormalBullet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

private:
	HRESULT ReadyBulletType(const BULLET_INFO& bulletInfo);

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag) override;
	virtual DWORD Free(void) override;
};
_END


#endif