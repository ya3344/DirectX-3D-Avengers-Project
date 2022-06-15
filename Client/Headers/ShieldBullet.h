#ifndef SHIELD_BULLET_H__ 
#define SHIELD_BULLET_H__

#include "Bullet.h"
#include "Include.h"

_BEGIN(Client)

class ShieldBullet final : public Bullet {

private:
	explicit ShieldBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit ShieldBullet(const ShieldBullet& rhs);
	virtual ~ShieldBullet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	bool IsEquip() { return m_bEquip; }
	bool IsActive() { return m_bActive; }
	void SetEquip(const bool bEquip) { m_bEquip = bEquip; }
	void SetActive(const bool bActive) { m_bActive = bActive; }
	HRESULT ResetShield(const BULLET_INFO& bulletInfo);

private:
	HRESULT ReadyBulletType(const BULLET_INFO& bulletInfo);
	void ReflectWall(D3DXVECTOR3 vWallSide);
	void SetUpWorld(const D3DXVECTOR3& vScale);

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual ShieldBullet* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag);
	virtual DWORD Free(void) override;

private:
	D3DXVECTOR3 m_vLook = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	float mRot = 0.f;
	int mRotDir = 0;
	float mRotAcc = 0.f;

private:
	bool m_bEquip = false; // Is In Hand Of Player
	bool m_bActive = false; // Hit by Boss or Not
};
_END


#endif