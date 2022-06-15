#ifndef EFFECT_H__
#define EFFECT_H__

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



class Planet final : public Engine::GameObject
{
private:
	explicit Planet(LPDIRECT3DDEVICE9 pDevice);
	explicit Planet(const Planet& rhs);
	virtual ~Planet() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT ReadyPrototype(const PLANET_INFO& objectInfo);
	void Set_PlanetInfo(const PLANET_INFO& planetInfo);
	void Set_CenterPos();
private:
	HRESULT ReadyComponent(void);
	void MakeMatrix();

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::CubeTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;

public:
	static Planet* Create(LPDIRECT3DDEVICE9 pDevice, const PLANET_INFO& planetInfo);
	virtual Planet* Clone() override;
	virtual DWORD Free() override;

private:
	PLANET_INFO mPlanetInfo;
	Engine::MATRIX_INFO m_matInfo;
	D3DXMATRIX m_matRevolRot;
	D3DXMATRIX m_matParent;
	D3DXMATRIX m_matRotX;
};

_END

#endif

