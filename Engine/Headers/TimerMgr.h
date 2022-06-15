#ifndef TIMER_MGR_H__
#define TIMER_MGR_H__

#include "Base.h"
_BEGIN(Engine)

class Timer;

class _ENGINE_DLL TimerMgr : public Base
{
	_DECLARE_SINGLETON(TimerMgr);

private:
	explicit TimerMgr();
	virtual ~TimerMgr() = default;

public:
	HRESULT AddTimer(const TCHAR* pTimerTag);
	const float Get_TimeDelta(const TCHAR* pTimerTag);

public:
	void ShowFPS(HWND hWnd, float* timeFPS);
	bool RangeUpdate(const float range, const float timeDelta);

private:
	Timer* FindTimer(const TCHAR* pTimerTag);

public:
	virtual DWORD Free(void);

private:
	map<const TCHAR*, Timer*> mTimerSpace;
	typedef map<const TCHAR*, Timer*> MAP_TIMER;
	typedef pair<const TCHAR*, Timer*> PAIR_TIMER;

private:
	double mTime = 0;
	float mTimeDelta = 0.f;
	size_t mTimeCount = 0;
	TCHAR mFPSBuf[32] = TEXT("");


};

_END
#endif