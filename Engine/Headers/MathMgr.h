#ifndef MATH_MGR_H__
#define MATH_MGR_H__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL MathMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(MathMgr)

public:
	enum OBJ_MOTION : size_t
	{
		LEFT = 0,
		LD,
		LU,
		UP,
		RIGHT,
		RU,
		RD,
		DOWN,
		MOTION_END
	};

private:
	explicit MathMgr();
	virtual ~MathMgr() = default;

public:
	const float CalDistance(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTarget);
	const float CalDirDegree(const D3DXVECTOR3& vSrcPos, const D3DXVECTOR3& vDestPos);

public:
	const D3DXVECTOR3& CalTargetDir(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTarget);
	const D3DXVECTOR3& RandomPos(const D3DXVECTOR3& vPos, float distance = 1.f);
	const D3DXVECTOR3& RandomDir();

public:
	virtual DWORD Free() override;

private: // RandomPos Related Vriable
	D3DXVECTOR3 m_vRandomPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vTargetDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vRandomDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	OBJ_MOTION mPreMotion = MOTION_END;
	OBJ_MOTION mCurMotion = MOTION_END;
};

_END

#endif

