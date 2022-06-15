#ifndef LAYER_H__
#define LAYER_H__

#include "Base.h"

_BEGIN(Engine)

class GameObject;
class Component;

class  _ENGINE_DLL  Layer final : public Base
{
private:
	explicit Layer();
	virtual ~Layer() = default;

public:
	HRESULT Init();
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

public:
	// 복합 컴포넌트 구조에서 따로 호출.(Player, Boss)
	void GameObjectRender();

public:
	Component* GetComponent(const size_t index, const TCHAR* pComponentTag);
	GameObject* GetGameObject(const TCHAR* pObjectName = nullptr);
	HRESULT Add_GameObject(GameObject* pGameObject);
	//JH_GetSpaceLayer_180615 Add Get_GameObjectSpace() 
	list<GameObject*>* Get_GameObjectSpace() { return &mGameObject_Space; }
	//TY_ClearLayer_180614 Add Function To Clear Layer
	void ClearLayer();

private:
	list<GameObject*> mGameObject_Space;

public:
	static Layer* Create();
	virtual DWORD Free() override;

};

_END

#endif
