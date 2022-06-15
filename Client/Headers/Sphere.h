#ifndef SPHERE_H__
#define SPHERE_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
}

_BEGIN(Client)

class Sphere final :public Engine::GameObject
{
public:
	explicit Sphere(LPDIRECT3DDEVICE9 pDevice);
	explicit Sphere(const Sphere& rhs);
	virtual ~Sphere() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT Ready_GameObjectPrototype();

private:
	HRESULT ReadyComponent(void);

public:
	void SetRadius(const float radius);
	void SetGameObject(Engine::Transform* pTransform) { m_pTargeInfo = pTransform; }

public:
	float GetRadius() const { return mRadius; }
	D3DXVECTOR3 GetCenterPos() const;

public:
	static Sphere* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;

private:
	LPD3DXMESH m_pSphereMesh = nullptr;

private:
	Engine::Transform* m_pTargeInfo = nullptr;
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	float mRadius = 0.f;

private:
	bool m_bDrawSphere = true;
};
_END
#endif
