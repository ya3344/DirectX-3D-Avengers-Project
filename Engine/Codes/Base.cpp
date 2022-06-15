#include "Base.h"

_USING(Engine)

Base::Base() 
	: mRefCnt(0)
{
}

DWORD Engine::Base::AddRef()
{
	return ++mRefCnt;
}

DWORD Engine::Base::Release()
{
	if (mRefCnt == 0)
	{
		Free();
		delete this;
		
		return 0;
	}

	return mRefCnt--;
}

