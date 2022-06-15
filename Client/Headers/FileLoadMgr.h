#ifndef FILE_LOADMGR_H__
#define FILE_LOADMGR_H__

#include "Base.h"

_BEGIN(Client)

class FileLoadMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(FileLoadMgr)

private:
	explicit FileLoadMgr();
	virtual ~FileLoadMgr() = default;

public:
	HRESULT Obj_FileLoad(LPDIRECT3DDEVICE9 pDevice, size_t sceneIdx, const TCHAR* pLayerTag, const TCHAR* pComTextureTag, const TCHAR* pFilePath);

public:
	virtual DWORD Free() override;

private: // FileRead Path Related Variable
	vector<wstring> mNameSpace;
};

_END

#endif // FILE_LOADMGR_H__