#ifndef CAMERA_MGR_H__
#define CAMERA_MGR_H__

#include "Base.h"

namespace Engine
{
	class KeyMgr;
}

_BEGIN(Client)

class CameraMgr final :public Engine::Base
{
	_DECLARE_SINGLETON(CameraMgr)
private:
	explicit CameraMgr();
	virtual ~CameraMgr() = default;

public:
	HRESULT Init();
	HRESULT Update_Camera();
	HRESULT ChangeCamera(TCHAR* pCameraTag);

public:
	HRESULT Ready_LayerCamera(TCHAR* pCameraTag);

public:
	void ShakeCamera(const float shakeTime);
	void InitActionCam(TCHAR* pPathName);
	//TY_TPS ZoomIn_180705_ Add ZoomCamera Function
	void ZoomCamera_TPS(const D3DXVECTOR3& vDest);

public:
	virtual DWORD Free() override;

private: //Camera Choice Related Variable
	TCHAR* m_pCurCamera_Tag = nullptr;
	TCHAR* m_pNewCamera_Tag = nullptr;
	Engine::KeyMgr* m_pKeyMgr = nullptr;
};
_END
#endif
