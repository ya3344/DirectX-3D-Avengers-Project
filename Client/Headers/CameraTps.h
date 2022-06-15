#ifndef CAMERA_TPS_H__
#define CAMERA_TPS_H__

#include "Camera.h"
#include "Include.h"

namespace Engine
{
	class Transform;
}

_BEGIN(Client)

class CameraTps final : public Engine::Camera
{
private:
	explicit CameraTps(LPDIRECT3DDEVICE9 pDevice);
	explicit CameraTps(const CameraTps& rhs);
	virtual ~CameraTps() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY, const float aspect, const float projNear, const float projFar) override;

	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;

public:
	HRESULT SetInfo();
	void SetZoom(const D3DXVECTOR3& vZoom);
public:
	void ZoomIn(const float timeDelta);
	void ZoomOut(const float timeDelta);
	void MoveToDest();

public:
	static CameraTps* Create(LPDIRECT3DDEVICE9 pDevce,
		const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY = D3DXToRadian(60.0f), const float aspect = float(WIN_SIZEX) / WIN_SIZEY, const float projNear = 0.2f, const float projFar = 1000.f); // 투영행렬을 만들기 위한 정보.

	virtual GameObject * Clone() override;
	virtual DWORD Free(void) override;

private: // JH_CameraTpsTarget_1800615 PlayerTransform Related Variable
	bool m_bGetPos = false;
	const SuperCube_Info* m_pPlayerInfo = nullptr;
	const SuperCube_Info* m_pBossInfo = nullptr;

private:
	float mXControl = 0.f;
	float mYControl = 0.f;
	float mZControl = 0.f;
	float mInterDist = 0.f;
	float mPace = 0.f;

	float mYCriteria = 0.f;

private:
	D3DXVECTOR3 m_vSavedPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vZoom = { 0.f, 0.f, 0.f };
	float mZoomTIme = 0.f;
	bool m_bZoom = false;
	int mZoomCount = 0;
	int mZoomCount_Max = 0;

private:
	D3DXVECTOR3 m_vDestEye = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vDestAt = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vCameraDir = { 0.f, 0.f, 0.f };

	bool bStageInit = false;
private:
	float mTimeDelta = 0.f;
};
_END
#endif // CAMERA_FREE_H__

