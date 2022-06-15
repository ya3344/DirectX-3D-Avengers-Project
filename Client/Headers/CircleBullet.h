#ifndef CIRCLE_BULLET_H__ 
#define CIRCLE_BULLET_H__ 

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class CircleBullet final : public Bullet {

private:
	explicit CircleBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CircleBullet(const CircleBullet& rhs);
	virtual ~CircleBullet() = default;

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
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag, const int dir);
	virtual DWORD Free(void) override;

private:
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vCenterDir = D3DXVECTOR3(0.f, 0.f, 0.f);

private:
	float mTimeDelta = 0.f;

private:
	D3DXMATRIX m_matParent;
	int mRotDir = 0;
	float mRotRadian = 0.f;
	float mRevolRadian = 0.f;
};
_END


#endif