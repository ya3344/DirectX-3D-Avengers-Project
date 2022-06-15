#ifndef WALL_BULLET_H__ 
#define WALL_BULLET_H__ 

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class WallBullet final : public Bullet {

private:
	explicit WallBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit WallBullet(const WallBullet& rhs);
	virtual ~WallBullet() = default;

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

private:
	float mRim = 0.f;
};
_END


#endif