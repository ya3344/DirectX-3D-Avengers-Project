#ifndef LOGO_H__
#define LOGO_H__

#include "Scene.h"

_BEGIN(Client)

class BackLogo;

class Logo final : public Engine::Scene
{
private:
	explicit Logo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Logo() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render(void) override;

public:
	HRESULT Ready_GameObjectPrototype();
	HRESULT ReadyComponent();
	HRESULT Ready_LayerBackGround(const TCHAR* pLayerTag);

public:
	static Logo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual DWORD Free();


};

_END

#endif
