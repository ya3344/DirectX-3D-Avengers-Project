#ifndef GAMEOBJECT_H__
#define GAMEOBJECT_H__

#include "Base.h"
#include "KeyMgr.h"

_BEGIN(Engine)

class Component;

class _ENGINE_DLL GameObject abstract : public Base
{
protected:
	explicit GameObject(LPDIRECT3DDEVICE9 pDevice);
	explicit GameObject(const GameObject& rhs);
	virtual ~GameObject() = default;

public:
	virtual HRESULT Init();
	virtual UPDATE Update(const float timeDelta);
	virtual UPDATE LastUpdate(const float tTimeDelta);
	virtual void Render();
	virtual HRESULT ReadyPrototype();

public:
	void Compute_ViewZ(const D3DXVECTOR3* pPositionInWorld);
	float GetViewZ(void) const { return mViewZ; }

public:
	Component* GetComponent(const TCHAR* pComponentTag) const;
	HRESULT AddComponent(const TCHAR* pComponentTag, Component* pComponent);

public:
	void setName(const wstring& objectName) { mObjectName = objectName; }
	const wstring& GetName() const { return mObjectName; }

private:
	Component* FindComponent(const TCHAR* pComponentTag) const;

public:
	virtual GameObject* Clone() = 0;
	virtual DWORD Free() override;

private:
	map<const TCHAR*, Component*> mComponentSpace;
	typedef map<const TCHAR*, Component*> MAP_COMPONENT;
	typedef pair<const TCHAR*, Component*> PAIR_COMPONENT;

private:
	float mViewZ = 0.f;

protected:
	wstring mObjectName = L"";

protected:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

protected:
	KeyMgr* m_pKeyMgr = nullptr;

};

_END

#endif // GAMEOBJECT_H__
