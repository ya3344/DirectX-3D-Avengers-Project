#ifndef TIMEMGR_H__
#define TIMEMGR_H__

#include "Base.h"

_BEGIN(Engine)

class Timer;

class _ENGINE_DLL TimeMgr : public Base
{
	_DECLARE_SINGLETON(TimeMgr);

private:
	explicit TimeMgr();
	virtual ~TimeMgr() = default;

public:
	void Init();
	void Update();
	
public:
	void ShowFPS();

public:
	virtual size_t Free() override;

private:
	Timer* m_pTimer = nullptr;

private:
	float mTime = 0.f;
	size_t mTimeCount = 0;
	TCHAR mFPSBuf[32] = TEXT("");
};

_END

#endif // TIMEMGR_H__