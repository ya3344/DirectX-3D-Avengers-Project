#ifndef CAMERA_H__
#define CAMERA_H__

#include "GameObject.h"
_BEGIN(Engine)

typedef struct tag_ViewInfo
{
	tag_ViewInfo() 
	{
		D3DXMatrixIdentity(&matView);
	}
	explicit tag_ViewInfo(D3DXMATRIX matView, D3DXVECTOR3 vEye, D3DXVECTOR3 vAt, D3DXVECTOR3 vAxisY)
	{
		this->matView = matView;
		this->vEye = vEye;
		this->vAt = vAt;
		this->vAxisY = vAxisY;
	}

	D3DXMATRIX matView;
	D3DXVECTOR3 vEye = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vAt = { 0.f , 0.f, 0.f };
	D3DXVECTOR3 vAxisY = { 0.f, 0.f, 0.f };

}VIEW_INFO;

typedef struct tag_ProjInfo
{
	tag_ProjInfo() 
	{
		D3DXMatrixIdentity(&matProj);
	}
	explicit tag_ProjInfo(D3DXMATRIX matProj, float fovY, float aspect, float projNear, float projFar)
	{
		this->matProj = matProj;
		this->fovY = fovY;
		this->aspect = aspect;
		this->projNear = projNear;
		this->projFar = projFar;
	}

	D3DXMATRIX matProj;
	float fovY = 0.f;
	float aspect = 0.f;
	float projNear = 0.f;
	float projFar = 0.f;

}PROJ_INFO;

class _ENGINE_DLL Camera abstract : public GameObject
{
protected:
	explicit Camera(LPDIRECT3DDEVICE9 pDevice);
	explicit Camera(const Camera& rhs);
	virtual ~Camera() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
						const float fovY, const float aspect, const float projNear, const float projFar);

	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;

	//DH_GetINFO_180613_Get_View,Proj_Information
public:
	const VIEW_INFO& GetViewInfo() const { return mViewInfo; }
	const PROJ_INFO& GetProjInfo() const { return mProjInfo; }

public: //TY_CameraShake_180624_Function To Shake Camera
	void SetCameraShake(const float shakeTime);

protected:
	void CameraShaking(const float timeDelta);


private:
	HRESULT Cal_ViewMatrix();
	HRESULT Cal_ProjMatrix();

public:
	virtual DWORD Free() override;

protected:
	VIEW_INFO mViewInfo;
	PROJ_INFO mProjInfo;
	D3DXMATRIX m_matWorld;

private: // Shaking Related Variable
	float mShakeTime = 0.f;
	int mShakeAcc_Count = 0;
	float mShakeInterval = 0.f;

protected:
	bool m_bShake = false;
};

_END

#endif

