#include "..\Headers\Timer.h"

_USING(Engine);

Timer::Timer()
{
}

HRESULT Timer::Init()
{
	QueryPerformanceCounter(&mFrameTime);
	QueryPerformanceCounter(&mFixTime);
	QueryPerformanceCounter(&mLastTime);
	QueryPerformanceFrequency(&mCpuTick);

	return NOERROR;
}

const float Timer::Get_TimeDelta()
{
	QueryPerformanceCounter(&mFrameTime);

	mTimeDelta = float(mFrameTime.QuadPart - mLastTime.QuadPart) / mCpuTick.QuadPart;

	mLastTime = mFrameTime;

	if (mFrameTime.QuadPart - mFixTime.QuadPart >= mCpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&mCpuTick);
		mFixTime = mFrameTime;
	}

	return mTimeDelta;
}

Timer * Timer::Create()
{
	Timer* pInstance = new Timer;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Timer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance; 

}

DWORD Timer::Free()
{
	return DWORD();
}

