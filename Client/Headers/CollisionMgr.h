#ifndef COLLISION_MGR_H__
#define COLLISION_MGR_H__

#include "Base.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class ParticleCubeMgr;
class Sphere;
class AnimCube;
class Player;
class Boss;
class CollisionMgr final :public Engine::Base
{
	_DECLARE_SINGLETON(CollisionMgr)

private:
	explicit CollisionMgr();
	virtual ~CollisionMgr() = default;

public:
	HRESULT Init();
	void Update();

public:
	static int OBB_Intersection(AnimCube* pCube1, AnimCube* pCube2);
	static int Sphere_Intersection(const Sphere* pSphere1, const Sphere* pSphere2);

public:
	virtual DWORD Free() override;

private:
	const list<Engine::GameObject*>* m_pBulletList = nullptr;
	Player* m_pPlayer = nullptr;
	Boss* m_pBoss = nullptr;

private:
	ParticleCubeMgr* m_pParticleMgr = nullptr;
};
_END
#endif
