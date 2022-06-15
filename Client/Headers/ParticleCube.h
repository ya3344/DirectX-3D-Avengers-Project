#ifndef PARTICLE_CUBE_H__
#define PARTICLE_CUBE_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class CubeTex;
	class Texture;

}

_BEGIN(Client)
class ParticleCube final : public Engine::GameObject
{
private:
	explicit ParticleCube(LPDIRECT3DDEVICE9 pDevice);
	explicit ParticleCube(const ParticleCube& rhs);
	virtual ~ParticleCube() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT Ready_GameObjectPrototype();

private:
	HRESULT ReadyComponent(void);

private:
	void SetWorldMatrix();

public:
	void SetDir(const D3DXVECTOR3& vDir);
	void SetPos(const D3DXVECTOR3& vPos);
	void SetColor(const int colorTexNum);
	void SetRange(const int range);

public:
	const bool IsDead() const { return mDead; }

public:
	static ParticleCube* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::CubeTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	float mAlpha = 0.f;

private:
	Engine::MATRIX_INFO mMatrixInfo;
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vDir = { 0.f, 0.f, 0.f };
	float mOriginY = 0.f;
	float mAccY = 0.f;
	float mRange = 0.f; // Variable Related To Range Of Distance 

private:
	float mAccTime = 0.f;
	bool mDead = false;

private:
	int texNum = 0;
};
_END
#endif //PARTICLE_CUBE_H__