#ifndef COMPONENTMGR_H__
#define COMPONENTMGR_H__

#include "Base.h"
#include "Transform.h"
#include "Renderer.h"
#include "RectColor.h"
#include "RectTex.h"
#include "CubeTex.h"
#include "CubeColor.h"
#include "TerrainBuffer.h"
#include "Texture.h"
#include "Picking.h"
#include "AnimNode.h"
#include "UITex.h" // TY_UI_180627_ Include Header


// 컴포넌트 객체들의 원형을 보관한다.
_BEGIN(Engine)

class _ENGINE_DLL ComponentMgr : public Base
{
public:
	_DECLARE_SINGLETON(ComponentMgr)

private:
	explicit ComponentMgr();
	virtual ~ComponentMgr() = default;

public:
	HRESULT ReserveContainer(const WORD size);
	HRESULT AddPrototype(const WORD sceneIdx, const TCHAR* pComponentTag, Component* pComponent);
	Component* Clone(const WORD sceneIdx, const TCHAR* pComponentTag);

private:
	Component* FindComponent(const WORD sceneIdx, const TCHAR* pComponentTag);

public:
	virtual DWORD Free(void);

private:
	map<const TCHAR*, Component*>* m_pPrototypeSpace = nullptr;
	typedef map<const TCHAR*, Component*> MAP_PROTOTYPE;
	typedef pair<const TCHAR*, Component*> PAIR_PROTOTYPE;

private:
	WORD mReserveSize = 0;
	bool m_bReserved = false;
};

_END

#endif //COMPONENTMGR_H__