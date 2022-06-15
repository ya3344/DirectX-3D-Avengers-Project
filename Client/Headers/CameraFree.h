#ifndef CAMERA_FREE_H__
#define CAMERA_FREE_H__

#include "Camera.h"
#include "Include.h"

_BEGIN(Client)

class CameraFree final : public Engine::Camera
{
private:
	explicit CameraFree(LPDIRECT3DDEVICE9 pDevice);
	explicit CameraFree(const CameraFree& rhs);
	virtual ~CameraFree() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY, const float aspect, const float projNear, const float projFar) override;

	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;

public: //TY_CAMERAPATH_180618 Function To Create ActionCamera Path
	void MakePath(const float timeDelta);

public:
	static CameraFree* Create(LPDIRECT3DDEVICE9 pDevce,
		const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY = D3DXToRadian(60.0f), const float aspect = float(WIN_SIZEX) / WIN_SIZEY, const float projNear = 0.2f, const float projFar = 1000.f); // 투영행렬을 만들기 위한 정보.

	virtual GameObject * Clone() override;
	virtual DWORD Free(void) override;

private: //CameraPath Related Variables
	bool m_bRecord = false;
	int mRecordTime = 0;
	size_t mPathNum = 0;
	float mPathUpdateTime = 0.f;
};

_END
#endif // CAMERA_FREE_H__

