#ifndef BULLET_H__ 
#define BULLET_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class CubeTex;
	class Texture;
}

_BEGIN(Client)

class Sphere;
class Bullet : public Engine::GameObject
{
protected:
	explicit Bullet(LPDIRECT3DDEVICE9 pDevice);
	explicit Bullet(const Bullet& rhs);
	virtual ~Bullet() = default;

public:
	virtual HRESULT Init() PURE;
	virtual UPDATE Update(const float timeDelta) PURE;
	virtual UPDATE LastUpdate(const float timeDelta) PURE;
	virtual void Render() PURE;

protected:
	HRESULT ReadyComponent();
	HRESULT ReadyPrototype();

public:
	Sphere* GetSphere() const { return m_pSphere; }
	void SetDead() { mDead = UPDATE::DIE; }
	const D3DXVECTOR3* GetPos() { return m_pMyPos; }
	const D3DXVECTOR3* GetDir();
	Engine::Transform* GetTransform() { return m_pTransform; }

public:
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag) { return nullptr; }
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const TCHAR* pSphereTag, const int recursiveCount) { return nullptr; }
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo, const TCHAR* pSphereTag) { return nullptr; } //TY_FixChangeTerrain_180704_ Add Clone
	virtual GameObject* Clone(const BULLET_INFO& bulletInfo, const SUPER_CUBE_INFO* pTargetInfo, const TCHAR* pSphereTag, const int dir) { return nullptr; }
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

protected: // Component Related Variable
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::CubeTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;
protected:
	Engine::SoundMgr* m_pSoundMgr = nullptr;
protected:
	const TCHAR * m_pTexturePath = nullptr;
	const D3DXVECTOR3* m_pMyPos = nullptr;

public:
	size_t mTextureNum = 0;

protected: // Bullet Value Related Variable
	BULLET_INFO mBulletInfo;

protected: // Sphere Related Variable
	Sphere* m_pSphere = nullptr;
	const TCHAR* m_pSphereTag = nullptr;

protected: // Delete Related Variable
	UPDATE mDead = UPDATE::NORMAL;

protected://
	D3DXMATRIX*	m_matBulletWorld = nullptr;
	D3DXMATRIX m_matTest;

};
_END
#endif