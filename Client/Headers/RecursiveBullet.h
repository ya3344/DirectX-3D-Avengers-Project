#ifndef TIME_BULLET_H__ 
#define TIME_BULLET_H__ 

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class RecursiveBullet final : public Bullet {

private:
	explicit RecursiveBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit RecursiveBullet(const RecursiveBullet& rhs);
	virtual ~RecursiveBullet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

private:
	HRESULT ReadyBulletType(const BULLET_INFO& bulletInfo);

public:
	int PlusRecursiveCount() { return ++mRecursiveCount; }

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag, const int recursiveCount) override;
	virtual DWORD Free(void) override;

private:
	int mRecursiveCount = 0;

private:
	float mTimeAcc = 0.f;
	float mTimeDead = 0.f;
	float mPaceAcc = 0.f;
};
_END


#endif