#ifndef ANIM_MGR_H__
#define ANIM_MGR_H__

#include "Component.h"
#include "AnimationStruct.h"

namespace Engine
{
	class Layer;
	class AnimNode;
	class GameObject;
}

_BEGIN(Client)

class AnimMgr final : public Engine::Component
{
private:
	explicit AnimMgr(LPDIRECT3DDEVICE9 pDevice);
	explicit AnimMgr(const AnimMgr& rhs);
	virtual ~AnimMgr() = default;

public:
	HRESULT Init();

public:
	void Set_AnimLayer(Engine::Layer* pLayer);

public: // Hierachy Related Function
	HRESULT SetHierachy();
	Engine::AnimNode* FindHierachy(const wstring& parentName);
	HRESULT Delete_HierachyCube(const wstring& cubeName);
	void RecursiveDelete(Engine::AnimNode* pAnimNode);
	HRESULT DeleteCube(const wstring& cubeName);
	HRESULT DeleteAllCube();

public: // AnimInfo Related Funciton
	bool Set_AnimInfo(const wstring& animName);
	void Set_FrameInfo(const FRAME_INFO& frameInfo);
	void Set_InitRot(const wstring& animName);
	void Set_InitMatrix(const wstring& animName);

public:
	void SetShiled_RenderState(const bool renderState);

public: // Get Anim, Frame Related Function
	const FRAME_INFO* Get_FrameInfo(const TCHAR* pCubeName) const;
	const ANIM_INFO* Get_AnimInfo(const TCHAR* pCubeName) const;

public: // AnimValue Related Funciton
	void Set_AnimSpeed(const float scaleSpeed, const float rotSpeed, const float transSpeed);

public:
	void Render();
	void InterpolationUpdate(const float timeDelta);

public:
	static AnimMgr* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual AnimMgr* Clone() override;
	virtual DWORD Free() override;

private:
	Engine::Layer* m_pAnimLayer = nullptr;
	list<Engine::GameObject*>* m_pGameObject_Space = nullptr;

};

_END

#endif

