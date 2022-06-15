#ifndef DNA_BULLET_H__ 
#define DNA_BULLET_H__ 

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class DNABullet final : public Bullet {

private:
	explicit DNABullet(LPDIRECT3DDEVICE9 pDevice);
	explicit DNABullet(const DNABullet& rhs);
	virtual ~DNABullet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

private:
	HRESULT ReadyBulletType(const BULLET_INFO& bulletInfo);
	void SetUpWorld_DNA();
	void SetUpWorld_Normal();


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
	int mRotDir = 0;
	float mRotDegree = 0.f;
	float mRange = 0.f;
	bool m_bDir = false;

private:
	bool m_bWallCollision = false;
	bool m_bDNACenter = false;
};
_END


#endif