#ifndef RENDERER_H__
#define RENDERER_H__

#include "Component.h"

// 그려야하는 객체들의 포인터를 보관한다.(모아서 관리한다.)
_BEGIN(Engine)

class GameObject;

class _ENGINE_DLL Renderer final : public Component
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

private:
	explicit Renderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Renderer() = default;

public:
	HRESULT Init();
	void RenderGameObject();
	HRESULT Add_RenderGroup(RENDERGROUP type, GameObject* pGameObject);

private:
	void RenderPriority();
	void RenderNonAlpha();
	void RenderAlpha();
	void RenderUI();
	
public:
	static Renderer* Create(LPDIRECT3DDEVICE9 pDevice);
	void ClearRenderGroup();
	virtual Component* Clone() override;
	virtual DWORD Free() override;


private:
	list<GameObject*> mObjectSpace[RENDER_END];


};


_END
#endif // !RENDERER_H__

