#ifndef BACKLOGO_H__
#define BACKLOGO_H__

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

class BackLogo final : public Engine::GameObject
{
private:
	explicit BackLogo(LPDIRECT3DDEVICE9 pDevice);
	explicit BackLogo(const BackLogo& rhs);
	virtual ~BackLogo() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT ReadyComponent();

public:
	static unsigned int _stdcall ThreadFunc(void* pArg);


public:
	static BackLogo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Engine::GameObject* Clone() override;
	virtual DWORD Free() override;


private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::RectTex* m_pRectTex = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	bool m_bNotRender = false;
	HANDLE m_hThread;
};

_END

#endif //BACKLOGO_H__
