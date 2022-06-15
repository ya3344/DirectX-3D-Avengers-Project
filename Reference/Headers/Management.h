#ifndef MANAGEMENT_H__
#define MANAGEMENT_H__

#include "Base.h"

_BEGIN(Engine)

class Scene;

class _ENGINE_DLL Management final : public Base
{
public:
	_DECLARE_SINGLETON(Management);

private:
	explicit Management();
	virtual ~Management() = default;

public:
	UPDATE Update(const float timeDelta);
	void Render(void);

public:
	HRESULT ReserveContainer(const WORD maxScene);
	HRESULT SetUp_CurrentScene(Scene* pNewScene);

public:
	virtual DWORD Free(void);

private:
	Scene* m_pCurrentScene = nullptr;

};

_END

#endif //MANAGEMENT_H__
