#ifndef TIMER_H__
#define TIMER_H__

#include "Base.h"

_BEGIN(Engine)

class Timer : public Base
{
private:
	explicit Timer();
	virtual ~Timer() = default;

public:
	HRESULT Init();

public:
	const float Get_TimeDelta();

public:
	static Timer* Create();
	virtual DWORD Free();

private:
	LARGE_INTEGER mFrameTime = {};
	LARGE_INTEGER mLastTime = {};
	LARGE_INTEGER mFixTime = {};
	LARGE_INTEGER mCpuTick = {};

private:
	double mTime = 0;
	float mTimeDelta = 0.f;

private:
	size_t mTimeCount = 0;
	TCHAR mFPSBuf[32] = TEXT("");

};

_END

#endif //TIMER_H__

