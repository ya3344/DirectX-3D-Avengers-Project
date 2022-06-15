#ifndef EFFECT_H__
#define EFFECT_H__

#include "GameObject.h"
#include "Include.h"


namespace Engine
{
	class Transform;
	class Renderer;
	class RectTex;
	class Texture;
}

_BEGIN(Client)

class Effect final : public Engine::GameObject
{
private:
	explicit Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit Effect(const Effect& rhs);
	virtual ~Effect() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT ReadyPrototype(const EFFECT_INIT_INFO& effect_InitInfo);

public:
	void EffectOperation(const EFFECT_INFO& effectInfo);

private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_BillBoard(void);

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::RectTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	float mFrame = 0.f;
	EFFECT_INIT_INFO mEffect_InitInfo;
	EFFECT_INFO mEffectInfo;
	bool m_bRender = false;

public:
	static Effect* Create(LPDIRECT3DDEVICE9 pDevice, EFFECT_INIT_INFO effect_InitInfo);
	virtual Effect* Clone() override;
	virtual DWORD Free() override;
};

_END

#endif

