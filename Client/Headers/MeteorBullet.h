#ifndef METEOR_BULLET_H__ 
#define METEOR_BULLET_H__ 

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class MeteorBullet final : public Bullet {

private:
	explicit MeteorBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit MeteorBullet(const MeteorBullet& rhs);
	virtual ~MeteorBullet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

private:
	HRESULT ReadyBulletType(const BULLET_INFO& bulletInfo);
	void SetUpWorld();

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag) override;
	virtual DWORD Free(void) override;

private:
	D3DXVECTOR3 m_vLook = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	float accPace = 0.f;
	float mRotRadian = 0.f;

private:
	float mCollisionTime = 0.f;
};
_END


#endif