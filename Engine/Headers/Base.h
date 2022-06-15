#ifndef BASE_H__
#define BASE_H__

#include "Engine_Include.h"

_BEGIN(Engine)

class _ENGINE_DLL Base abstract
{
protected:
	explicit Base();
	virtual ~Base() = default;

public:
	DWORD AddRef();
	DWORD Release();

private:
	DWORD mRefCnt = 0;

public:
	virtual DWORD Free() = 0;
};
_END

#endif // BASE_H__

