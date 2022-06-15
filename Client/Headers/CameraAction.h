#ifndef CAMERA_ACTION_H__
#define CAMERA_ACTION_H__

#include "Camera.h"
#include "PathData.h"
#include "Include.h"

_BEGIN(Client)

class CameraAction final : public Engine::Camera
{
private:
	explicit CameraAction(LPDIRECT3DDEVICE9 pDevice);
	explicit CameraAction(const CameraAction& rhs);
	virtual ~CameraAction() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY, const float aspect, const float projNear, const float projFar) override;

	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;

public:
	void SetPathData(TCHAR* pathName);

private:
	void FollowPath(const float timeDelta);

public:
	static CameraAction* Create(LPDIRECT3DDEVICE9 pDevce,
		const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY = D3DXToRadian(60.0f), const float aspect = float(WIN_SIZEX) / WIN_SIZEY, const float projNear = 0.2f, const float projFar = 1000.f); // 투영행렬을 만들기 위한 정보.

	virtual GameObject * Clone() override;
	virtual DWORD Free(void) override;

private:
	size_t m_curFrame = 0;

private:
	vector<PATH_DATA*> mPathData_Space;
	size_t mPathNum = 0;
	float mPathUpdateTime = 0.f;

private:
	float mMovePace = 0.f;
	float mRotPace = 0.f;
};
_END
#endif
