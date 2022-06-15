#pragma once

#ifndef SKYBOX_H__ 
#define SKYBOX_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class CubeTex;
	class CubeColor;
	class Texture;
}


_BEGIN(Client)

class SkyBox final: public Engine::GameObject
{
private:
	explicit SkyBox(LPDIRECT3DDEVICE9 pDevice);
	explicit SkyBox(const SkyBox& rhs);
	virtual ~SkyBox() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT ReadyComponent(void);

public:
	static SkyBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::CubeTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	D3DXVECTOR3 m_vTargetPos = { 0.f, 0.f, 0.f };
	TCHAR mFileName[10][MAX_PATH] = {};
};

_END
#endif