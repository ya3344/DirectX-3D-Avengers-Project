#ifndef ANIM_CUBE_H__
#define ANIM_CUBE_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class CubeTex;
	class Texture;
	class AnimNode;
}

_BEGIN(Client)

class AnimCube final : public Engine::GameObject
{
private:
	explicit AnimCube(LPDIRECT3DDEVICE9 pDevice);
	explicit AnimCube(const AnimCube& rhs);
	virtual ~AnimCube() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT Ready_GameObjectPrototype(const ANIM_CUBE_INFO& animCube_Info);

public:
	void SetTextureNum(const size_t textureNum) { mTextureNum = textureNum; }
	void SetShiledRender(const bool bRender) { m_bShiledRender = bRender; }

public:
	Engine::AnimNode* Get_AnimNode() { return m_pAnimNode; }
	Engine::Transform* GetTransform() { return m_pTransform; }

private:
	HRESULT ReadyComponent(const TCHAR* pComTextureTag, const WORD comSceneIdx);

public:
	static AnimCube* Create(LPDIRECT3DDEVICE9 pDevice, const ANIM_CUBE_INFO& animCube_Info);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::CubeTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;
	Engine::AnimNode* m_pAnimNode = nullptr;

private:
	bool m_bAlreadyComponent = true;

private:
	bool m_bShiledRender = false;
	size_t mTextureNum = 0;

};

_END

#endif

