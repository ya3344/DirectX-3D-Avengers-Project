#ifndef BULLET_MGR_H__
#define BULLET_MGR_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class Bullet;

class BulletMgr final : public Engine::Base
{
public:
	enum BULLET_OBJ { PLAYER, BOSS, OBJ_END };

public:
	_DECLARE_SINGLETON(BulletMgr)

private:
	explicit BulletMgr();
	virtual ~BulletMgr() = default;

public:
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

	void SetPlayerInfo(const SUPER_CUBE_INFO* pPlayerInfo) { m_pPlayerInfo = pPlayerInfo; }
	void SetBossInfo(const SUPER_CUBE_INFO* pBossInfo) { m_pBossInfo = pBossInfo; }

public:
	HRESULT AddBullet(BULLET_OBJ obj, const BULLET_INFO& bulletInfo);
	HRESULT Add_DirBullet(BULLET_OBJ obj, const BULLET_INFO& bulletInfo, const int dir); // DNA, Circle, Digda
	HRESULT Add_RecursiveBullet(BULLET_OBJ obj, const BULLET_INFO& bulletInfo, const int recursiveCount);
	HRESULT Add_DigdaBullet(BULLET_OBJ obj, const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo, const int dir);
	HRESULT Add_SearchBullet(BULLET_OBJ obj, const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo);
	HRESULT Ready_BulletPrototype(LPDIRECT3DDEVICE9 pDevice);

public: // TY_OBB_180619 Add Function To Get BulletList
	const list<Engine::GameObject*>* GetBulletList() const { return m_pBulletSpace; }

private:
	void KeyCheck();

public:
	virtual DWORD Free() override;

private:
	list<Engine::GameObject*> m_pBulletSpace[OBJ_END];

	const SUPER_CUBE_INFO* m_pPlayerInfo;
	const SUPER_CUBE_INFO* m_pBossInfo;

private:
	UPDATE mUpdate = UPDATE::FAIL;

private: // Time Related Varialbe 
	float mTimeDelta = 0.f;

private: // TY_OBB_180619 Add Sphere Tag Related Variable
	vector<wstring> mTagSpace;
	int mCount = 0;
};

_END

#endif // BULLET_MGR_H__